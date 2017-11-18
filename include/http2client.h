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
#ifndef HTTP2CLIENT_H
#define HTTP2CLIENT_H

#include "network.h"
#include "logger.h"
#include "nghttp2multiplexhandler.h"
#include "httpresponse.h"
#include "http2clienteventmanager.h"

using namespace Network::HTTP;

namespace Network
{
    namespace HTTP2
    {
        enum StreamQueueEvent
        {
            EVENT_STREAM_SYNCSTATE ,
            EVENT_STREAM_PING ,
            EVENT_STREAM_RECOGNICE ,
            EVENT_STREAM_SETTINGS_UPDATE ,
            EVENT_STREAM_SPEECH_SYNTHESIZER_START ,
            EVENT_STREAM_SPEECH_SYNTHESIZER_FINISH,

            EVENT_STREAM_PLAYBACK_STARTED  ,
            EVENT_STREAM_PLAYBACK_STOPPED  ,
            EVENT_STREAM_PLAYBACK_FINISHED ,
            EVENT_STREAM_PLAYBACK_FAILED   ,
            EVENT_STREAM_PLAYBACK_PAUSED   ,
            EVENT_STREAM_PLAYBACK_RESUME   ,
            EVENT_STREAM_PLAYBACK_NEARLY_FINISHED ,
            EVENT_STREAM_PROGRESS_REPORT_DELAY_ELAPSED ,
            EVENT_STREAM_PROGRESS_REPORT_INTERVAL_ELAPSED ,
            EVENT_STREAM_PLAYBACK_STUTTER_STARTED ,
            EVENT_STREAM_PLAYBACK_STUTTER_FINISHED ,
            EVENT_STREAM_PLAYBACK_QUEUE_CLEARED
        };

        class HTTP2Client : public NgHTTP2MultiplexHandler , public NgHTTP2ResponseListener
        {
            public:
               HTTP2Client( const string url );
               ~HTTP2Client();

               void initConnection( nghttp2_nv * header_data , size_t header_len , int * stream_status );
               void pushStreamRequest( StreamQueueEvent event );

               HTTP2ClientEventManager * getEventManager(void);
               HTTPResponse * getResponse(void);

               void onResponseEvent( HTTP2HandlerResponse * response );
               void onResponseHeaderStatusEvent( const char * status_code );
               void stream_loop( struct event_base * evbase );

               virtual StreamQueueData * onProcessStreamEvent( StreamQueueEvent event ) = 0;
               virtual void onStreamLoopStartEvent(void)=0;
               virtual void onStreamLoopWaitEvent(void)=0;

            protected:

                std::string m_base_url;

                HTTP2HandlerResponse    * m_http2_handler_response;
                NgHTTP2ResponseManager  * m_http2_response_manager;
                HTTP2ClientEventManager * m_http2_event_manager;
                HTTPResponse            * m_http_output_response;


            private:

                boost::shared_ptr< std::queue< StreamQueueEvent > >  m_stream_queue;
                boost::mutex m_mutex;
                boost::condition_variable m_cv;

                bool m_response_event_ignore;
        };
    }
}
#endif // HTTP2CLIENT_H
