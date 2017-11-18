/*
   Copyright (C) 2017 - 2018 this.mutzii@gmail.com

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "include/nghttp2api.h"

using namespace Network::HTTP2;

NgHTTP2API::NgHTTP2API(){}

NgHTTP2API::~NgHTTP2API(){}

void NgHTTP2API::setReadCallback(HTTP2ResponseHeaderCallback header_callback, HTTP2ResponseBodyCallback body_callback, void *user_data)
{
    m_response_header_callback = header_callback;
    m_response_body_callback   = body_callback;
    m_response_user_data       = user_data;
}

int NgHTTP2API::select_next_proto_cb(SSL *ssl, unsigned char **out, unsigned char *outlen, const unsigned char *in, unsigned int inlen, void *arg)
{
    if( nghttp2_select_next_protocol(out , outlen , in , inlen) <= 0) {
         Logger::instance().error("Server did not advertise");
    }

    return SSL_TLSEXT_ERR_OK;
}

SSL_CTX * NgHTTP2API::create_ssl_ctx(void)
{
    SSL_CTX *ssl_ctx;
    ssl_ctx = SSL_CTX_new( SSLv23_client_method() );

    if (!ssl_ctx) {
        Logger::instance().error( ErrorException("Could not create SSL/TLS context:", ERR_error_string(ERR_get_error(), NULL) ));
    }

    SSL_CTX_set_options(ssl_ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
                                 SSL_OP_NO_COMPRESSION |
                                 SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);
    SSL_CTX_set_next_proto_select_cb(ssl_ctx ,&NgHTTP2API::select_next_proto_cb, NULL );

    #if OPENSSL_VERSION_NUMBER >= 0x10002000L
    SSL_CTX_set_alpn_protos(ssl_ctx, (const unsigned char *)"\x02h2", 3);
    #endif // OPENSSL_VERSION_NUMBER >= 0x10002000L

    return ssl_ctx;
}

SSL * NgHTTP2API::create_ssl(SSL_CTX *ssl_ctx)
{
    SSL *ssl;
    ssl = SSL_new(ssl_ctx);

    if (!ssl)
        Logger::instance().error( ErrorException("Could not create SSL/TLS session object:", ERR_error_string(ERR_get_error(), NULL) ));

    stringstream ssl_version_output;
    ssl_version_output << "SSL version: " << SSL_get_version(ssl );
    Logger::instance().info( ssl_version_output.str() );

    return ssl;
}

void NgHTTP2API::initiate_connection(event_base *evbase, SSL_CTX *ssl_ctx, const char *host, uint16_t port, http2_session_data *session_data)
{
     int rv;
     struct bufferevent *bev;
     SSL *ssl;

     ssl = create_ssl(ssl_ctx);

     bev = bufferevent_openssl_socket_new( evbase, -1, ssl, BUFFEREVENT_SSL_CONNECTING, BEV_OPT_DEFER_CALLBACKS | BEV_OPT_CLOSE_ON_FREE);

     bufferevent_setcb(bev, &NgHTTP2API::readcb, &NgHTTP2API::writecb, &NgHTTP2API::eventcb , session_data);

     rv = bufferevent_socket_connect_hostname(bev, session_data->dnsbase, AF_UNSPEC, host, port);

     if (rv != 0) {
       Logger::instance().error( ErrorException("Could not connect to the remote host:", host ));
     }

     session_data->bev = bev;
}

void NgHTTP2API::eventcb(bufferevent *bev, short events, void *ptr)
{
    http2_session_data *session_data = (http2_session_data *)ptr;

      if (events & BEV_EVENT_CONNECTED)
      {

        int fd = bufferevent_getfd(bev);
        int val = 1;
        const unsigned char *alpn = NULL;
        unsigned int alpnlen = 0;
        SSL *ssl;

        Logger::instance().info("Connected to host ...");

        ssl = bufferevent_openssl_get_ssl(session_data->bev);

        SSL_get0_next_proto_negotiated(ssl, &alpn, &alpnlen);

            #if OPENSSL_VERSION_NUMBER >= 0x10002000L
            if (alpn == NULL) {
                SSL_get0_alpn_selected(ssl, &alpn, &alpnlen);
            }
            #endif // OPENSSL_VERSION_NUMBER >= 0x10002000L

            if (alpn == NULL || alpnlen != 2 || memcmp("h2", alpn, 2) != 0) {

                Logger::instance().error("ALPN protocol: h2 is not negotiated");

                delete_http2_session_data(session_data);

                return;
            }

        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&val, sizeof(val));

        initialize_nghttp2_session(session_data);

        send_client_connection_header(session_data);

        submit_request(session_data);

        session_send( session_data );

        Logger::instance().info("Connected");
        return;
      }

      if (events & BEV_EVENT_EOF) {
          Logger::instance().warn("Disconnected from the remote host");
      } else if (events & BEV_EVENT_ERROR) {
          Logger::instance().warn("Network error");
      } else if (events & BEV_EVENT_TIMEOUT) {
          Logger::instance().warn("Timeout");
      }

      delete_http2_session_data(session_data);
}

void NgHTTP2API::initialize_nghttp2_session(http2_session_data *session_data)
{
     nghttp2_session_callbacks *callbacks;

     nghttp2_session_callbacks_new( &callbacks );

     nghttp2_session_callbacks_set_send_callback(callbacks, &NgHTTP2API::send_callback );

     nghttp2_session_callbacks_set_on_frame_recv_callback(callbacks, &NgHTTP2API::on_frame_recv_callback);

     nghttp2_session_callbacks_set_on_frame_send_callback( callbacks , &NgHTTP2API::on_frame_send_callback);

     nghttp2_session_callbacks_set_on_data_chunk_recv_callback( callbacks, &NgHTTP2API::on_data_chunk_recv_callback);

     nghttp2_session_callbacks_set_on_stream_close_callback( callbacks, &NgHTTP2API::on_stream_close_callback);

     nghttp2_session_callbacks_set_on_header_callback(callbacks, &NgHTTP2API::on_header_callback);

     nghttp2_session_callbacks_set_on_begin_headers_callback( callbacks, &NgHTTP2API::on_begin_headers_callback);

     nghttp2_session_client_new(&session_data->session, callbacks, session_data);

     nghttp2_session_callbacks_del(callbacks);
}

void NgHTTP2API::send_client_connection_header(http2_session_data *session_data)
{
    nghttp2_settings_entry iv[1] = { {NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 1000} };
    int rv;

    /* client 24 bytes magic string will be sent by nghttp2 library */
    rv = nghttp2_submit_settings(session_data->session, NGHTTP2_FLAG_NONE, iv, ARRLEN(iv) );

    if (rv != 0) {
         Logger::instance().error( ErrorException("Could not submit SETTINGS", nghttp2_strerror(rv)) );
    }
}

void NgHTTP2API::submit_request( http2_session_data * session_data )
{
    int32_t stream_id = 0;
    http2_stream_data *stream_data = session_data->stream_data;

    if( stream_data->data_provider != nullptr || stream_data->data_provider != NULL)
    {
        stream_data->data_provider->read_callback = &NgHTTP2API::data_source_read_callback;
    }

    stream_id = nghttp2_submit_request( session_data->session,            // nghttp2_session
                                        NULL,                             // nghttp2_priority_spec
                                        stream_data->data_header,         // nghttp2_nv
                                        stream_data->data_header_size ,   // size of nghttp2_nv
                                        stream_data->data_provider    ,   // nghttp2_data_provider
                                        stream_data);                     // stream_user_data

    if (stream_id < 0) {
         Logger::instance().error( ErrorException("Could not submit HTTP Request:", nghttp2_strerror( stream_id )));
    }

    Logger::instance().info("submit HTTP Request - SUCCESS");
    stream_data->stream_id = stream_id;
    Logger::instance().info("NgHTTP2API-StreamID:" , stream_data->stream_id );
}

void NgHTTP2API::submit_data( http2_session_data * session_data )
{
    if( session_data->stream_data->data_provider != nullptr || session_data->stream_data->data_provider != NULL)
    {
        session_data->stream_data->data_provider->read_callback = &NgHTTP2API::data_source_read_callback;
    }

    int rv;
    rv = nghttp2_submit_data( session_data->session , NGHTTP2_FLAG_END_STREAM , session_data->stream_data->stream_id , session_data->stream_data->data_provider );

    if (rv != 0) {
        Logger::instance().warn( ErrorException("Fatal error:", nghttp2_strerror(rv)));
    } else {
        Logger::instance().info("submit data FRAME - SUCCESS ");
    }
}

void NgHTTP2API::readcb(bufferevent *bev, void *ptr)
{
    http2_session_data *session_data = (http2_session_data *)ptr;
    ssize_t readlen;
    struct evbuffer *input = bufferevent_get_input(bev);
    size_t datalen         = evbuffer_get_length(input);
    unsigned char *data    = evbuffer_pullup(input, -1);

    readlen = nghttp2_session_mem_recv(session_data->session, data, datalen);

    if (readlen < 0) {
        Logger::instance().warn( ErrorException("Fatal error:", nghttp2_strerror((int)readlen)));
        delete_http2_session_data(session_data);
        return;
    }

    if (evbuffer_drain(input, (size_t)readlen) != 0) {

        Logger::instance().warn("Fatal error: evbuffer_drain failed");
        delete_http2_session_data(session_data);
        return;
    }

    if (session_send(session_data) != 0) {

        delete_http2_session_data(session_data);
        return;
    }
}

void NgHTTP2API::writecb(bufferevent *bev, void *ptr)
{
    // NgHTTP2API::session_send contained nghttp2_session_send function
    // nghttp2_session_send function => invoke NgHTTP2API::send_callback was contained bufferevent_write
    // bufferevent_write    function => invoke this callback function ( i think so )

    http2_session_data *session_data = (http2_session_data *)ptr;

    // if session wants to receive data from the remote peer.
    int read  = nghttp2_session_want_read(session_data->session);

    // if session wants to send data to the remote peer.
    int write = nghttp2_session_want_write(session_data->session);

    if ( read == 0 && write == 0 && evbuffer_get_length( bufferevent_get_output(session_data->bev)) == 0)
    {
        //if both nghttp2_session_want_read() and nghttp2_session_want_write() return 0, the application should drop the connection
        delete_http2_session_data(session_data);
    }
}

int NgHTTP2API::session_send(http2_session_data *session_data)
{
    int rv;
    rv = nghttp2_session_send( session_data->session );

    if (rv != 0) {

        Logger::instance().warn( ErrorException("Fatal error:", nghttp2_strerror(rv)));
        return -1;
    }

  return 0;
}

ssize_t NgHTTP2API::data_source_read_callback(nghttp2_session *session, int32_t stream_id, uint8_t *buf, size_t length, uint32_t *data_flags, nghttp2_data_source *source, void *user_data)
{
    http2_session_data *session_data = (http2_session_data *)user_data;

    if (session_data->stream_data->stream_id == stream_id )
    {
        // The implementation of this function must read at most length bytes of data from source
        // and store them in buf and return number of data stored in buf.
        // If EOF is reached, set NGHTTP2_DATA_FLAG_EOF flag in *data_flags.

        void * data = source->ptr;
        size_t data_source_len        =  session_data->stream_data->data_source_len;
        size_t data_source_offset_len =  session_data->stream_data->data_source_offset_len;
        ssize_t nread = 0;

        if( data != NULL || data != nullptr )
        {
              // clear the buf
              memset( buf , 0 , length );

              if( length < ( data_source_len - data_source_offset_len ) )
              {
                  memcpy( buf , data + data_source_offset_len , length );

              } else {

                  length = data_source_len - data_source_offset_len;
                  memcpy( buf , data + data_source_offset_len , length );
              }

              // add length to data_source_offset
              session_data->stream_data->data_source_offset_len += length;

              // EOF is reached
              if( session_data->stream_data->data_source_offset_len >= data_source_len )
              {
                 *data_flags |= NGHTTP2_DATA_FLAG_EOF;
              }

              // return number of data stored in buf
              nread = length;

              // return number of data stored in buf
              Logger::instance().info("return number of data stored in buf", nread );
              return nread;
        }
    }

  return 0;
}

ssize_t NgHTTP2API::send_callback(nghttp2_session *session, const uint8_t *data, size_t length, int flags, void *user_data)
{
    http2_session_data *session_data = (http2_session_data *)user_data;
    struct bufferevent *bev = session_data->bev;
    bufferevent_write(bev, data, length);
    return (ssize_t)length;
}

int NgHTTP2API::on_header_callback(nghttp2_session *session, const nghttp2_frame *frame, const uint8_t *name, size_t namelen, const uint8_t *value, size_t valuelen, uint8_t flags, void *user_data)
{
    http2_session_data *session_data = (http2_session_data *)user_data;

    switch (frame->hd.type) {

        case NGHTTP2_HEADERS:

            if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE && session_data->stream_data->stream_id == frame->hd.stream_id) {

                //@Todo Response Header - Response Header Callback here ...
                //print_header(stderr, name, namelen, value, valuelen);
                session_data->api->m_response_header_callback( name , namelen , value , valuelen , session_data->api->m_response_user_data );
            }

        break;
    }

    return 0;
}

int NgHTTP2API::on_frame_recv_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data)
{
    http2_session_data *session_data = ( http2_session_data *)user_data;

    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
                    if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE && session_data->stream_data->stream_id == frame->hd.stream_id) {
                        Logger::instance().info("HEADER FRAME received");
                    }
        break;

        case NGHTTP2_DATA:
                    Logger::instance().info("Chunk DATA FRAME received");
        break;

    }
   return 0;
}

int NgHTTP2API::on_frame_send_callback( nghttp2_session * session , const nghttp2_frame * frame , void * user_data )
{
    http2_session_data *session_data = ( http2_session_data *) user_data;

    switch (frame->hd.type)
    {
        case NGHTTP2_HEADERS:
                    if (frame->headers.cat == NGHTTP2_HCAT_REQUEST && session_data->stream_data->stream_id == frame->hd.stream_id) {

                          nghttp2_nv * header_ptr = frame->headers.nva;
                          size_t  header_len      = frame->headers.nvlen;
                          nghttp2_nv * show_ptr   = header_ptr;
                          stringstream show_stream;
                          unsigned int counter = 0;

                          show_stream << "::Show (Send) Headers::" << std::endl;
                          while( show_ptr < header_ptr + header_len )
                          {
                              show_stream << "< " << (const char *) show_ptr->name << "(" << show_ptr->namelen << ") ";
                              show_stream << (const char *) show_ptr->value << " (" << show_ptr->valuelen << ")" << std::endl;
                              show_ptr++;
                              counter++;
                          }

                          Logger::instance().info( show_stream.str() );
                          Logger::instance().info("All headers send");
                    }
        break;

        case NGHTTP2_DATA:

                    if( session_data->stream_data->stream_id == frame->hd.stream_id )
                    {
                        Logger::instance().info("Chunk DATA FRAME send");
                    }
        break;

    }

    return 0;
}

int NgHTTP2API::on_data_chunk_recv_callback(nghttp2_session *session, uint8_t flags, int32_t stream_id, const uint8_t *data, size_t len, void *user_data)
{
    http2_session_data *session_data = ( http2_session_data *) user_data;

    if (session_data->stream_data->stream_id == stream_id )
    {
        // Callback function invoked when a chunk of data in DATA frame is received.
        // The flags is the flags of DATA frame which this data chunk is contained.

        if( session_data->stream_data->response_data->datalen == 0 ||
            session_data->stream_data->response_data->data == NULL )
        {

            uint8_t * chunk_buffer = (uint8_t *) malloc ( sizeof( uint8_t ) * len );

            memcpy( chunk_buffer , data , len );

            session_data->stream_data->response_data->data    = chunk_buffer;

            session_data->stream_data->response_data->datalen = len;


        } else {

            // old buffer size
            size_t old_buffer_size = session_data->stream_data->response_data->datalen;

            // old_buffer_size + chunk length
            size_t new_buffer_size = old_buffer_size + len;

            // old buffer
            uint8_t * old_buffer = session_data->stream_data->response_data->data;

            // create new_buffer
            uint8_t * new_buffer = ( uint8_t * ) malloc( sizeof( uint8_t ) * new_buffer_size );

            // copy old data into new buffer
            memcpy( new_buffer , old_buffer , old_buffer_size );

            // copy chunk data into new buffer (add)
            memcpy( new_buffer + old_buffer_size , data , len );

            // delete old_buffer
            free( old_buffer );

            // update / set : new_buffer & new_buffer_size
            session_data->stream_data->response_data->data     = new_buffer;

            session_data->stream_data->response_data->datalen  = new_buffer_size;

        }
    }

   return 0;
}

int NgHTTP2API::on_begin_headers_callback(nghttp2_session *session, const nghttp2_frame *frame, void *user_data)
{
    http2_session_data *session_data = (http2_session_data *)user_data;
    (void)session;

      switch (frame->hd.type)
      {
            case NGHTTP2_HEADERS:
                    if (frame->headers.cat == NGHTTP2_HCAT_RESPONSE && session_data->stream_data->stream_id == frame->hd.stream_id)
                    {
                        Logger::instance().info("Response headers for stream ID=", frame->hd.stream_id );
                    }
            break;
      }

   return 0;
}

int NgHTTP2API::on_stream_close_callback(nghttp2_session *session, int32_t stream_id, uint32_t error_code, void *user_data)
{
    http2_session_data *session_data = (http2_session_data *)user_data;
    int rv;

      if (session_data->stream_data->stream_id == stream_id) {

          Logger::instance().info("Stream closed with id:", stream_id );
          Logger::instance().info("with error_code:", error_code );

          Logger::instance().debug("-- START RESPONSE CALLBACK --");
          http2_response_chunk_data * response_data = session_data->stream_data->response_data;

          session_data->api->m_response_body_callback( response_data->data , response_data->datalen , session_data->api->m_response_user_data );

          if( response_data->data != NULL )
          {
              Logger::instance().info("free() - on_stream_close_callback ");
              free( response_data->data );
              response_data->data = NULL;
              response_data->datalen = 0;
          }

      }
      else if( session_data->stream_data->stream_id == 1 )
      {
          //Downchannel stream here ...
          Logger::instance().info("Stream closed with id:", stream_id );
          Logger::instance().info("with error_code:", error_code );
          Logger::instance().info("Terminate HTTP2 Session");

          rv = nghttp2_session_terminate_session( session, error_code );
          delete_http2_session_data( session_data );

          if (rv != 0) {
              return NGHTTP2_ERR_CALLBACK_FAILURE;
          }
      }

   return 0;
}

http2_session_data * NgHTTP2API::create_http2_connection_session_data( event_base *evbase , NgHTTP2API * api )
{
    http2_session_data * session_data = new http2_session_data();
    session_data->dnsbase = evdns_base_new( evbase , 1 );
    session_data->api     = api;
    return session_data;
}

http2_session_data * NgHTTP2API::create_http2_session_data(struct evdns_base * dnsbase, NgHTTP2API *api)
{
    http2_session_data * session_data = new http2_session_data();
    session_data->dnsbase = dnsbase;
    session_data->api     = api;
    return session_data;
}

void NgHTTP2API::delete_http2_session_data( http2_session_data *session_data )
{
    SSL *ssl = bufferevent_openssl_get_ssl(session_data->bev);

    if (ssl) {
      SSL_shutdown(ssl);
    }

    bufferevent_free(session_data->bev);
    session_data->bev = nullptr;

    evdns_base_free(session_data->dnsbase, 1);
    session_data->dnsbase = nullptr;

    nghttp2_session_del(session_data->session);
    session_data->session = nullptr;
}

void NgHTTP2API::print_header(FILE *f, const uint8_t *name, size_t namelen, const uint8_t *value, size_t valuelen)
{
    fwrite(name, 1, namelen, f);
    fprintf(f, ": ");
    fwrite(value, 1, valuelen, f);
    fprintf(f, "\n");
}

void NgHTTP2API::print_headers(FILE *f, nghttp2_nv *nva, size_t nvlen)
{
    size_t i;

    for (i = 0; i < nvlen; ++i) {
       print_header(f, nva[i].name, nva[i].namelen, nva[i].value, nva[i].valuelen);
    }

    fprintf(f, "\n");
}
