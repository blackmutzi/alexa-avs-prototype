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
#ifndef HTTP2CLIENTEVENTMANAGER_H
#define HTTP2CLIENTEVENTMANAGER_H

#include "network.h"
#include "httpclientdirectiveevent.h"
#include "directivenlistener.h"
#include "httpresponse.h"
#include "json.h"
#include "jsonreaderdirective.h"
#include "logger.h"
#include "http2multipartanalyse.h"

using namespace AVSJson;
using namespace Network::HTTP2;

namespace Network
{
    namespace HTTP2
    {
            using DirectivenListenerList  = std::list< DirectivenListener * >;

            using AudioDirectivenList     = std::list< AudioPlayerDirectivenListener * >;

            enum LINES
            {
                LINE_POSITION_0 ,
                LINE_POSITION_1 ,
                LINE_POSITION_2 ,
                LINE_POSITION_3 ,
                LINE_POSITION_4 ,
                LINE_POSITION_5
            };



            class HTTP2ClientEventManager
            {
                 public:
                    HTTP2ClientEventManager();
                    virtual ~HTTP2ClientEventManager();

                    void registerDirectivenListener( DirectivenListener * listener );
                    void unregisterDirectivenListener( DirectivenListener * listener );


                    void registerAudioDirectivenListener( AudioPlayerDirectivenListener * listener );
                    void unregisterAudioDirectivenListener( AudioPlayerDirectivenListener * listener );


                    virtual void invokeAllEvents( HTTPResponse * response );

                    void setDownChannelStatus( bool status );
                    int * getStreamStatus(void);
                    void setBoundary( std::string boundary );

                    /*!
                     * \brief getDownChannelStatus
                     * \return if true  downchannel is up
                     *         if false downchannel is down
                     */
                    bool getDownChannelStatus(void);

                protected:

                    void invokeRecognizeEvent( HTTPClientRecognizeDirective * listener , HTTPClientDirectiveEvent * event );

                    HTTPClientDirectiveEvent * http_response_analyse( HTTPResponse * response );

                    DirectivenListenerList m_directiven_listener_list;

                    AudioDirectivenList m_audio_directiven_listener_list;


                    int * m_stream_status;
                    bool m_downchannel_status;

                    std::string m_boundary;



            };
    }
}
#endif // HTTPCLIENTEVENTMANAGER_H
