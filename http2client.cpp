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
#include "include/http2client.h"

using namespace Network::HTTP2;

HTTP2Client::HTTP2Client(const string url)
    : NgHTTP2MultiplexHandler() ,
      NgHTTP2ResponseListener() ,
      m_base_url( url ) ,
      m_http2_handler_response  ( new HTTP2HandlerResponse())    ,
      m_http2_response_manager  ( new NgHTTP2ResponseManager())  ,
      m_http2_event_manager     ( new HTTP2ClientEventManager() ),
      m_http_output_response    ( new HTTPResponse() ) ,
      m_response_event_ignore( false )
{
    m_stream_queue.reset( new std::queue< StreamQueueEvent>() );
    m_http2_response_manager->registerResponseListener( this );
    NgHTTP2MultiplexHandler::initialize( m_http2_handler_response , m_http2_response_manager );
}

HTTP2Client::~HTTP2Client()
{
    delete m_http2_handler_response;

    delete m_http2_response_manager;

    delete m_http2_event_manager;

    delete m_http_output_response;
}

void HTTP2Client::initConnection( nghttp2_nv * header_data , size_t header_len , int * stream_status )
{
    NgHTTP2MultiplexHandler::initiateStream( m_base_url.c_str() , header_data , header_len , stream_status );
}

void HTTP2Client::pushStreamRequest( StreamQueueEvent event )
{
    std::queue< StreamQueueEvent > * queue = m_stream_queue.get();
    queue->push( event );

    m_cv.notify_one();
}

HTTP2ClientEventManager * HTTP2Client::getEventManager()
{
    return m_http2_event_manager;
}

HTTPResponse * HTTP2Client::getResponse(void)
{
    return m_http_output_response;
}

void HTTP2Client::onResponseEvent( HTTP2HandlerResponse *response )
{
    Logger::instance().info("-- ON_RESPONSE_EVENT_HAS_STARTED --");

    if(  !m_response_event_ignore )
    {

        m_http_output_response->setContent( response->memory );
        m_http_output_response->setSize( response->size );

        m_http2_event_manager->invokeAllEvents( m_http_output_response );

        m_http_output_response->setContent("");
        m_http_output_response->setSize(0);

    } else {

        Logger::instance().info(" -- RESPONSE_EVENT_CANCELED -- ");
        m_response_event_ignore = false;
    }
}

void HTTP2Client::onResponseHeaderStatusEvent(const char *status_code)
{
    Logger::instance().info("http2client status code check:");

    if( strcmp ( status_code, "200") == 0)
    {
        Logger::instance().info("Status Code 200 (ok)");

    } else if ( strcmp( status_code, "400") == 0 ){
        Logger::instance().info("Status Code 400 (Bad Request - Json Content is Bad) ");
        m_response_event_ignore = true;

    } else if ( strcmp( status_code, "403") == 0 ){
        Logger::instance().info("Status Code 403 (forbidden) ");

    } else if ( strcmp( status_code, "204") == 0 ){
         Logger::instance().info("Status Code 204 (no content) ");
         m_response_event_ignore = true;

    } else if( strcmp( status_code, "500") == 0 ){
         Logger::instance().info("Status Code 500 - Internal Server Error");
         m_response_event_ignore = true;
    } else {

    }
}

void HTTP2Client::stream_loop( struct event_base * evbase )
{
    boost::unique_lock< boost::mutex > lk( m_mutex );
    std::queue< StreamQueueEvent > * queue = m_stream_queue.get();
    onStreamLoopStartEvent();

    if( queue->empty() )
    {
         onStreamLoopWaitEvent();
         m_cv.wait( lk );
    }

    if( queue->empty() )
            return;

    StreamQueueEvent event = queue->front();
    queue->pop();

    boost::shared_ptr< StreamQueueData > stream_data( onProcessStreamEvent( event ) );
    createNewStream( stream_data.get() , evbase );
}
