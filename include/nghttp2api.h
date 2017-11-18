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
#ifndef NGHTTP2API_H
#define NGHTTP2API_H

#include "network.h"
#include "logger.h"

#define OUTPUT_WOULDBLOCK_THRESHOLD (1 << 16)
#define ARRLEN(x) (sizeof(x) / sizeof(x[0]))

#define MAKE_NV(NAME, VALUE)                                                   \
  {                                                                            \
    (uint8_t *)NAME, (uint8_t *)VALUE, sizeof(NAME) - 1, sizeof(VALUE) - 1,    \
       NGHTTP2_NV_FLAG_NO_COPY_NAME | NGHTTP2_NV_FLAG_NO_COPY_VALUE                                                   \
  }

#define MAKE_NV2(NAME, VALUE, VALUELEN)                                         \
  {                                                                            \
    (uint8_t *)NAME, (uint8_t *)VALUE, sizeof(NAME) - 1, VALUELEN,             \
        NGHTTP2_NV_FLAG_NO_COPY_NAME | NGHTTP2_NV_FLAG_NO_COPY_VALUE                                                 \
  }

namespace Network
{
    namespace HTTP2
    {

        struct http2_response_chunk_data {

            uint8_t * data;
            size_t datalen;

        };

        struct http2_stream_data {

            /* data provider */
            nghttp2_data_provider * data_provider;

            /* HTTP2 Header (Array) */
            nghttp2_nv * data_header;

            /* recv response data */
            http2_response_chunk_data * response_data;

            /* data_source_len */
            size_t data_source_len;

            /* data_source_offset_len*/
            size_t data_source_offset_len;

            /* HTTP2 Header size */
            // size = sizeof( x ) / sizeof( x[0] )
            size_t data_header_size;

            /* The stream ID of this stream */
            int32_t stream_id;

            /* data_source_read_callback offset */
            off_t data_offset;

        };

        struct http2_session_data {

            nghttp2_session       *session;

            struct evdns_base     *dnsbase;

            struct bufferevent    *bev;

            http2_stream_data     *stream_data;

            NgHTTP2API            *api;

        };

        typedef void (*HTTP2ResponseHeaderCallback)( const void * name , size_t name_size , const void * value , size_t value_size , void * userdata );
        typedef void (*HTTP2ResponseBodyCallback)  ( void * content , size_t size , void * userdata );

        class NgHTTP2API
        {
            public:
                NgHTTP2API();
                virtual ~NgHTTP2API();

                /*!
                 * Set Response Callbacks
                 * \brief setReadCallback
                 * \param header    : HeaderCallback
                 * \param body      : BodyCallback
                 * \param user_data : custom ptr to save / copy response data
                 */
                void setReadCallback( HTTP2ResponseHeaderCallback header, HTTP2ResponseBodyCallback body , void * user_data );

                /*!
                 * Create SSL CTX
                 * \brief create_ssl_ctx
                 * \return
                 */
                SSL_CTX * create_ssl_ctx(void);

                /*!
                 * Initiate Connection to the remote peer
                 * \brief initiate_connection
                 * \param evbase
                 * \param ssl_ctx
                 * \param host
                 * \param port
                 * \param session_data
                 */
                void initiate_connection( struct event_base * evbase ,
                                                 SSL_CTX * ssl_ctx          ,
                                                 const char * host          ,
                                                 uint16_t port              ,
                                                 http2_session_data * session_data
                                              );
                /*!
                 * Create HTTP2 Session
                 * \brief create_http2_session_data
                 * \param evbase
                 * \return
                 */
                http2_session_data * create_http2_session_data( struct evdns_base * dnsbase , NgHTTP2API *api );
                http2_session_data * create_http2_connection_session_data( struct event_base * evbase , NgHTTP2API * api );

            protected:

                HTTP2ResponseHeaderCallback m_response_header_callback;
                HTTP2ResponseBodyCallback m_response_body_callback;
                void * m_response_user_data;

            /*!
             * Callback function for create_ssl_ctx
             * \brief select_next_proto_cb : callback function
             * \param ssl
             * \param out
             * \param outlen
             * \param in
             * \param inlen
             * \param arg
             * \return
             */
            static int select_next_proto_cb( SSL *ssl ,
                                             unsigned char **out     ,
                                             unsigned char *outlen   ,
                                             const unsigned char *in ,
                                             unsigned int inlen     ,
                                             void *arg
                                           );
            /*!
             * Create SSL function
             * Invoke from : NgHTTP2API::initiate_connection
             * \brief create_ssl
             * \param ssl_ctx
             * \return
             */
            static SSL * create_ssl( SSL_CTX * ssl_ctx );

            /*!
             * BufferEvent - Managed Events
             *
             * BEV_EVENT_CONNECTED
             * BEV_EVENT_EOF
             * BEV_EVENT_ERROR
             * BEV_EVENT_TIMEOUT
             *
             * \brief eventcb : callback function
             * \param bev
             * \param events
             * \param ptr     : http_session_data
             */
            static void eventcb( struct bufferevent * bev ,
                                 short events ,
                                 void * ptr
                              );

            /*!
             * incoming data from the remote peer
             * \brief readcb : callback function
             * \param bev
             * \param ptr    : http_session_data
             */
            static void readcb( struct bufferevent * bev , void * ptr);

            /*!
             * send data to the remote peer
             * \brief writecb : callback function
             * \param bev
             * \param ptr     : http_session_data
             */
            static void writecb( struct bufferevent * bev , void  * ptr);

            /*!
             * Create nghttp2_session
             *
             * and setup callbacks
             * \brief initialize_nghttp2_session
             * \param session_data
             */
            static void initialize_nghttp2_session( http2_session_data * session_data );

            /*!
             * \brief send_client_connection_header
             * \param session_data
             */
            static void send_client_connection_header( http2_session_data * session_data );

            /*!
             * make stream id
             */
            static void submit_request( http2_session_data * session_data );

            /*!
             * set Source data for new stream id
             * \brief submit_data
             * \param session_data
             */
            static void submit_data( http2_session_data * session_data );

            /*!
             * send all data to the remote peer
             * invoke: send_callback
             *
             * \brief session_send
             * \param session_data
             * \return
             */
            static int session_send( http2_session_data * session_data );

            /*!
             * Read Data Source ( Body Request )
             *
             * \brief data_source_read_callback : callback function
             * \param session
             * \param stream_id
             * \param buf
             * \param length
             * \param data_flags
             * \param source
             * \param user_data : http2_session_data
             * \return
             */
            static ssize_t data_source_read_callback( nghttp2_session * session ,
                                                      int32_t stream_id ,
                                                      uint8_t * buf     ,
                                                      size_t length     ,
                                                      uint32_t * data_flags ,
                                                      nghttp2_data_source * source ,
                                                      void * user_data
                                                    );
            /*!
             * send all data to the remote peer
             * invoke: bufferevent_write
             *
             * \brief send_callback : callback function
             * \param session
             * \param data
             * \param length
             * \param flags
             * \param user_data : http2_session_data
             * \return
             */
            static ssize_t send_callback( nghttp2_session * session ,
                                          const uint8_t * data ,
                                          size_t length ,
                                          int flags ,
                                          void * user_data
                                       );
            /*!
             * Callback function invoked when a header name/value pair
             * is received for the frame.
             *
             * \brief on_header_callback : callback function
             * \param session
             * \param frame
             * \param name
             * \param namelen
             * \param value
             * \param valuelen
             * \param flags
             * \param user_data : http2_session_data
             * \return
             */
            static int on_header_callback( nghttp2_session * session ,
                                           const nghttp2_frame * frame ,
                                           const uint8_t * name ,
                                           size_t namelen ,
                                           const uint8_t * value ,
                                           size_t valuelen ,
                                           uint8_t flags ,
                                           void * user_data
                                         );
           /*!
            * Callback function invoked by nghttp2_session_recv() and
            * nghttp2_session_mem_recv() when a frame is received.
            *
            * \brief on_frame_recv_callback : callback function
            * \param session
            * \param frame
            * \param user_data : http2_session_data
            * \return
            */
           static int on_frame_recv_callback( nghttp2_session * session ,
                                              const nghttp2_frame * frame ,
                                              void * user_data
                                            );

           static int on_frame_send_callback( nghttp2_session * session ,
                                             const nghttp2_frame * fame ,
                                             void * user_data
                                            );
           /*!
            * Callback function invoked when a chunk of data in DATA frame is received.
            *
            * \brief on_data_chunk_recv_callback : callback function
            * \param session
            * \param flags
            * \param stream_id
            * \param data
            * \param len
            * \param user_data : http2_session_data
            * \return
            */
           static int on_data_chunk_recv_callback( nghttp2_session * session ,
                                                   uint8_t flags ,
                                                   int32_t stream_id ,
                                                   const uint8_t * data ,
                                                   size_t len ,
                                                   void * user_data
                                                 );
           /*!
            * Callback function invoked when the stream stream_id is closed
            *
            * \brief on_stream_close_callback
            * \param session
            * \param stream_id
            * \param error_code
            * \param user_data : http2_session_data
            * \return
            */
           static int on_stream_close_callback( nghttp2_session * session ,
                                                int32_t stream_id ,
                                                uint32_t error_code ,
                                                void * user_data
                                              );
           /*!
            * Callback function invoked when the reception of header block in HEADERS or PUSH_PROMISE is started.
            * Each header name/value pair will be emitted by nghttp2_on_header_callback.
            *
            * \brief on_begin_headers_callback : callback function
            * \param session
            * \param frame
            * \param user_data : http2_session_data
            * \return
            */
           static int on_begin_headers_callback( nghttp2_session * session   ,
                                                 const nghttp2_frame * frame ,
                                                 void * user_data
                                               );


           static void delete_http2_session_data( http2_session_data * session_data);

           static void print_header( FILE * f ,
                                     const uint8_t * name ,
                                     size_t namelen ,
                                     const uint8_t *value ,
                                     size_t valuelen
                                   );

           static void print_headers( FILE * f ,
                                      nghttp2_nv * nva ,
                                      size_t nvlen
                                    );
        };

    }
}

#endif // NGHTTP2API_H
