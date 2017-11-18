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
#include "include/nghttp2handler.h"

using namespace Network::HTTP2;

NgHTTP2Handler::NgHTTP2Handler()
    : m_session( nullptr ) ,
      m_header_copy( nullptr ) ,
      m_has_header( false )    ,
      m_has_source( false )    ,
      m_init( false )          ,
      NgHTTP2API()
{}
NgHTTP2Handler::~NgHTTP2Handler()
{}

void NgHTTP2Handler::initialize( HTTP2HandlerResponse * response , NgHTTP2ResponseManager * manager ) throw ( std::exception )
{
    setResponseManager( manager );

    m_session = new HTTP2HandlerSession();

    m_session->ssl_ctx = NgHTTP2API::create_ssl_ctx();

    m_session->evbase  = event_base_new();

    m_session->port = 443;

    m_session->session_data = NgHTTP2API::create_http2_connection_session_data( m_session->evbase , this );

    m_session->session_data->stream_data = new http2_stream_data();

    m_session->session_data->stream_data->data_offset = 0;

    m_session->session_data->stream_data->response_data = new http2_response_chunk_data();

    m_session->session_data->stream_data->response_data->data = NULL;

    m_session->session_data->stream_data->response_data->datalen = 0;

    m_session->session_data->stream_data->data_provider = NULL;


    m_session_response = response;
    m_session_response->memory = nullptr;
    m_session_response->size   = 0;

    m_session_response_management = new HTTP2HandlerResponseManagement();
    m_session_response_management->manager = m_response_manager;
    m_session_response_management->response_memory = response;

    NgHTTP2API::setReadCallback( &NgHTTP2Handler::onResponseHeaderEvent , &NgHTTP2Handler::onResponseBodyEvent , m_session_response_management );
    m_init = true;
}

void NgHTTP2Handler::onResponseHeaderEvent( const void *name, size_t name_size, const void *value, size_t value_size, void *userdata)
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

void NgHTTP2Handler::onResponseBodyEvent(void *content, size_t size, void *userdata)
{
    HTTP2HandlerResponseManagement * response_management = ( HTTP2HandlerResponseManagement * ) userdata;
    HTTP2HandlerResponse   * response         = response_management->response_memory;
    NgHTTP2ResponseManager * response_manager = response_management->manager;

    response->memory = ( char* ) malloc( response->size + size + 1 );
    memcpy( &(response->memory[response->size]) , content , size );

    response->size += size;
    response->memory[ response->size ] = 0;

    response_manager->createResponseEvent( response );

    free( response->memory );
}

void NgHTTP2Handler::setResponseManager( NgHTTP2ResponseManager * response_manager )
{
    m_response_manager = response_manager;
}

void NgHTTP2Handler::setUri(const char *uri) throw (std::exception )
{
    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setUri: HTTP2HandlerSession is null");

    if( m_session->session_data == nullptr)
            throw ErrorException("NullPointerException by setUri: http2_session_data is null");

    if( m_session->session_data->stream_data == nullptr )
            throw ErrorException("NullPointerException by setUri: http2_stream_data is null");

    m_session->host = uri;
}

void NgHTTP2Handler::setPort(uint16_t port) throw( std::exception )
{
    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setPort: HTTP2HandlerSession is null");

     m_session->port = port;
}
void NgHTTP2Handler::setRequestHeader( nghttp2_nv * data , size_t arr_len ) throw ( std::exception )
{
    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setRequestHeader: HTTP2HandlerSession is null");

    if( m_session->session_data == nullptr)
            throw ErrorException("NullPointerException by setRequestHeader: http2_session_data is null");

    if( m_session->session_data->stream_data == nullptr )
            throw ErrorException("NullPointerException by setRequestHeader: http2_stream_data is null");

    m_header_copy = new nghttp2_nv[ arr_len ];
    memcpy( m_header_copy , data , sizeof( nghttp2_nv) * arr_len );

    m_session->session_data->stream_data->data_header = m_header_copy;
    m_session->session_data->stream_data->data_header_size = arr_len;

    m_has_header = true;
}

void NgHTTP2Handler::setRequestSource( const std::string source ) throw ( std::exception )
{
    const char * data = source.c_str();
    size_t data_len   = strlen( data );

    if( m_session == nullptr )
            throw ErrorException("NullPointerException by setRequestMultipart: HTTP2HandlerSession is null");

    if( m_session->session_data == nullptr)
            throw ErrorException("NullPointerException by setRequestMultipart: http2_session_data is null");

    if( m_session->session_data->stream_data == nullptr )
            throw ErrorException("NullPointerException by setRequestMultipart: http2_stream_data is null");

    m_session->session_data->stream_data->data_provider = new nghttp2_data_provider();
    m_session->session_data->stream_data->data_source_offset_len = 0;
    m_session->session_data->stream_data->data_source_len = data_len;

    m_session->session_data->stream_data->data_provider->source.ptr = (void*) malloc ( data_len );
    memcpy( m_session->session_data->stream_data->data_provider->source.ptr , (void *) data , data_len );

    m_has_source = true;
}

void NgHTTP2Handler::perform(void) throw ( std::exception )
{
    struct event_base * evbase        = m_session->evbase;
    SSL_CTX * ssl_ctx                 = m_session->ssl_ctx;
    const char * host                 = m_session->host;
    uint16_t port                     = m_session->port;
    http2_session_data * session_data = m_session->session_data;

    try{

      NgHTTP2API::initiate_connection( evbase , ssl_ctx , host , port , session_data );

      event_base_loop( evbase , 0);

      event_base_free( evbase );

      SSL_CTX_free( ssl_ctx );

      cleanUp();

    } catch ( ErrorException e )
    {
        Logger::instance().info("NgHTTP2API Exception:");
    }
}

void NgHTTP2Handler::cleanUp(void) throw ( std::exception )
{
    if( m_init )
    {
        if( m_has_header )
        {
            delete[] m_header_copy;
        }

        if( m_has_source )
        {
            free( m_session->session_data->stream_data->data_provider->source.ptr );

            delete m_session->session_data->stream_data->data_provider;
        }

        delete m_session->session_data->stream_data->response_data;

        delete m_session->session_data->stream_data;

        delete m_session->session_data;

        delete m_session;

        delete m_session_response_management;
    }
}



