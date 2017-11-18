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
#ifndef HTTPHEADERFACTORY_H
#define HTTPHEADERFACTORY_H

#include "network.h"
#include "httpheader.h"
#include "http2header.h"

namespace Network
{
    namespace HTTP
    {

    /**
     * HTTP Request: Alexa Voice Service
     *
     *  Host: https://avs-alexa-eu.amazon.com
     *
     *
     *  Request-Events:
     *  URL: https://avs-alexa-eu.amazon.com/v20160207/events
     *  Events_Endpoint: /v20160207/events
     *  HTTPMethod: POST
     *  Content: metadata
     *           audio
     *
     *  Directive:
     *  URL: https://avs-alexa-eu.amazon.com/v20160207/directives
     *  Directives_Endpoint: /v20160207/directives
     *  HTTPMethod: GET
     *
     */

        class HTTPHeaderFactory
        {
            public:
               HTTPHeaderFactory();
               ~HTTPHeaderFactory(){}

                HTTP2Header create_HTTP2_POST( const std::string access_token , const std::string event_or_directive , const std::string boundary );
                HTTP2Header create_HTTP2_GET ( const std::string access_token , const std::string event_or_directive , const std::string boundary );

                HTTPHeader createJSONHeader(const std::string boundary);
                HTTPHeader createAudioHeader(const std::string boundary);


                static std::string EVENTS_ENDPOINT;
                static std::string DIRECTIVES_ENDPOINT;
                static std::string PING_ENDPOINT;


            private:


        };
    }
}
#endif // HTTPHEADERFACTORY_H
