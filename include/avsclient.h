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
#ifndef AVSCLIENT_H
#define AVSCLIENT_H

#include "stdafx.h"
#include "network.h"
#include "http2client.h"
#include "avsrequestbuilder.h"
#include "audiocapture.h"
#include "avsclientlistener.h"

using namespace Network::HTTP;
using namespace Audio;

using AVSListenerList = std::list< AVSClientListener * >;

class AVSClient : public HTTP2Client
{
    public:

        AVSClient(const std::string url , const std::string access_token );
        ~AVSClient();

        void addListener( AVSClientListener * listener );
        void removeListener( AVSClientListener * listener );

        void start(void);
        void restart(void);
        void changeAccessToken( const std::string access_token );

        AVSRequestBuilder * getRequestBuilder(void);
        AVSJson::SynchronizeStateEvent * getSyncState(void);

        bool isActive(void);
        StreamQueueData * onProcessStreamEvent(StreamQueueEvent event);
        void onStreamLoopStartEvent(void);
        void onStreamLoopWaitEvent(void);

    private:

        static void downchannel_process( AVSClient * client );
        static void ping_process( AVSClient * client );

        AVSRequestBuilder * m_avs_request_builder;
        AVSJson::SynchronizeStateEvent * m_sync_event;

        std::string m_base_url;
        bool m_active;

        AVSListenerList m_avs_client_listener_list;
};

#endif // AVSCLIENT_H
