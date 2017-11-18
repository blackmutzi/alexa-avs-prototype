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
#include "include/nghttp2multiplexhandler.h"

using namespace Network::HTTP2;

NgHTTP2MultiplexHandler::NgHTTP2MultiplexHandler()
    : NgHTTP2API() ,

      m_session( new HTTP2HandlerSession() ) ,
      m_session_response_management( new HTTP2HandlerResponseManagement()) ,
      m_init( false )

{}
NgHTTP2MultiplexHandler::~NgHTTP2MultiplexHandler(){}

void NgHTTP2MultiplexHandler::initialize(HTTP2HandlerResponse *response, NgHTTP2ResponseManager *manager) throw ( std::exception )
{
    SSL_load_error_strings();

    SSL_library_init();

    // Setup HTTP2_HANDLER_SESSION
    m_session->ssl_ctx = NgHTTP2API::create_ssl_ctx();

    m_session->evbase  = event_base_new();

    m_session->port = 443; //HTTPS Port

    m_session->session_data = NgHTTP2API::create_http2_connection_session_data( m_session->evbase , this );
    m_session->session_data->stream_data = nullptr;

    // Prepare / Init HTTP2_HANDLER_RESPONSE
    response->memory = nullptr;
    response->size   = 0;

    // Setup Response Management
    m_session_response_management->manager = manager;
    m_session_response_management->response_memory = response;

    // Setup Callbacks
    NgHTTP2API::setReadCallback( &NgHTTP2MultiplexHandler::onResponseHeaderEvent , &NgHTTP2MultiplexHandler::onResponseBodyEvent , m_session_response_management );

    // Finish Setup
    m_init = true;
}

void NgHTTP2MultiplexHandler::initiateStream( const char * uri , nghttp2_nv * header_data , size_t header_len , int * status )
{
    // Use Active Session
    // Create Main Stream Data
    m_session->session_data->stream_data = new http2_stream_data();
    m_session->session_data->stream_data->data_header = header_data;
    m_session->session_data->stream_data->data_offset = 0;
    m_session->session_data->stream_data->response_data = new http2_response_chunk_data();
    m_session->session_data->stream_data->response_data->data = NULL;
    m_session->session_data->stream_data->response_data->datalen = 0;
    m_session->session_data->stream_data->data_source_len = 0;
    m_session->session_data->stream_data->data_source_offset_len = 0;
    m_session->session_data->stream_data->data_header_size = header_len;
    m_session->session_data->stream_data->stream_id = 0;
    m_session->session_data->stream_data->data_offset =0;
    m_session->session_data->stream_data->data_provider = nullptr;

    // Setup Main Stream
    setUri( uri );

    struct event_base * evbase        = m_session->evbase;
    SSL_CTX * ssl_ctx                 = m_session->ssl_ctx;
    const char * host                 = m_session->host;
    uint16_t port                     = m_session->port;
    http2_session_data * session_data = m_session->session_data;
    bool main_stream_loop = true;

      NgHTTP2API::initiate_connection( evbase , ssl_ctx , host , port , session_data );

      event_base_loop( evbase , EVLOOP_NONBLOCK);

      struct timeval ten_sec;
      ten_sec.tv_sec  = 5;
      ten_sec.tv_usec = 0;

      event_base_loopexit(evbase, &ten_sec);
      event_base_dispatch(evbase);

      if( session_data->session == nullptr)
      {
          Logger::instance().info("Session Terminated! Connection Lost.");
          main_stream_loop = false;
      }

      Logger::instance().info("--- Stream Loop has started ---");
      *status = STREAM_LOOP_HAS_STARTED;
      while( main_stream_loop )
      {
          if( session_data->session == nullptr )
                break;

          stream_loop( evbase );
      }
      Logger::instance().info("--- Stream Loop has stopped ---");
      *status = STREAM_SESSION_TERMINATED;

      event_base_free( evbase );

      SSL_CTX_free( ssl_ctx );

      cleanUp();
}
void NgHTTP2MultiplexHandler::createNewStream( StreamQueueData * data ,  struct event_base * evbase )
{
    // save old stream
    http2_stream_data * old_stream_ptr = m_session->session_data->stream_data;

    // MAKE HEADER STREAM DATA
    m_session->session_data->stream_data = new http2_stream_data();
    m_session->session_data->stream_data->data_provider = new nghttp2_data_provider();
    m_session->session_data->stream_data->data_header   = data->header_data;
    m_session->session_data->stream_data->response_data = new http2_response_chunk_data();

    m_session->session_data->stream_data->response_data->data = NULL;
    m_session->session_data->stream_data->response_data->datalen = 0;

    m_session->session_data->stream_data->data_source_len = data->source_len;
    m_session->session_data->stream_data->data_source_offset_len = 0;
    m_session->session_data->stream_data->data_header_size = data->header_len;
    m_session->session_data->stream_data->stream_id = 0;
    m_session->session_data->stream_data->data_offset =0;
    m_session->session_data->stream_data->data_provider->source.ptr = (void*) data->source_data;

    // MAKE HEADER STREAM
    NgHTTP2API::submit_request( m_session->session_data );

    // Send ALL FRAMES to remote peer
    NgHTTP2API::session_send  ( m_session->session_data );

    //do it

    struct timeval ten_sec;
    ten_sec.tv_sec  = 5;
    ten_sec.tv_usec = 0;

    event_base_loopexit(evbase, &ten_sec);
    event_base_dispatch(evbase);
    // FINISH
    // CLEAN UP HERE

    // DELETE SOURCE STREAM DATA
    delete m_session->session_data->stream_data->response_data;
    delete m_session->session_data->stream_data->data_provider;
    delete m_session->session_data->stream_data;

    // FALLBACK TO DOWNCHANNEL STREAM DATA
    m_session->session_data->stream_data = old_stream_ptr;
}

void NgHTTP2MultiplexHandler::onResponseHeaderEvent( const void *name, size_t name_size, const void *value, size_t value_size, void *userdata)
{
    (void) name_size;
    (void) value_size;

    HTTP2HandlerResponseManagement * response_management = ( HTTP2HandlerResponseManagement * ) userdata;
    NgHTTP2ResponseManager * response_manager = response_management->manager;

    stringstream header_info_stream;
    header_info_stream << (const char*) name << " " << (const char*) value << std::endl;
    Logger::instance().debug( header_info_stream.str() );

    if( strcmp( (const char *) name , ":status") == 0 )
    {
        response_manager->createResponseHeaderStatusEvent( (const char*) value );
    }
}

void NgHTTP2MultiplexHandler::onResponseBodyEvent(void *content, size_t size, void *userdata)
{
    HTTP2HandlerResponseManagement * response_management = ( HTTP2HandlerResponseManagement * ) userdata;
    HTTP2HandlerResponse   * response         = response_management->response_memory;
    NgHTTP2ResponseManager * response_manager = response_management->manager;
    size_t memory_size = size + 1;

    response->memory = new char[ memory_size ];
    memset( response->memory , 0 , memory_size );
    memcpy( response->memory , content , size );
    response->size = size;

    response_manager->createResponseEvent( response );

    delete[] response->memory;
    response->size = 0;
}

void NgHTTP2MultiplexHandler::setUri(const char *uri) throw (std::exception )
{
    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setUri: HTTP2HandlerSession is null");

    if( m_session->session_data == nullptr)
            throw ErrorException("NullPointerException by setUri: http2_session_data is null");

    if( m_session->session_data->stream_data == nullptr )
            throw ErrorException("NullPointerException by setUri: http2_stream_data is null");

    m_session->host = uri;
}

void NgHTTP2MultiplexHandler::setPort(uint16_t port) throw( std::exception )
{
    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setPort: HTTP2HandlerSession is null");

     m_session->port = port;
}

void NgHTTP2MultiplexHandler::cleanUp(void) throw ( std::exception )
{
    if( m_init )
    {
        delete m_session->session_data->stream_data->response_data;

        delete m_session->session_data->stream_data;

        delete m_session->session_data;

        delete m_session;

        delete m_session_response_management;
    }
}



