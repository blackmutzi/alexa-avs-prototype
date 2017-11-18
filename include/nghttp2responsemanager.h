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
#ifndef NGHTTP2RESPONSEMANAGER_H
#define NGHTTP2RESPONSEMANAGER_H

#include "network.h"
#include "logger.h"
#include "nghttp2handler.h"

namespace Network
{
    namespace HTTP2
    {
        using NgHTTP2ResponseListenerList = std::list < NgHTTP2ResponseListener * >;

        class NgHTTP2ResponseManager
        {
             public:
                NgHTTP2ResponseManager();
                ~NgHTTP2ResponseManager(){}

                void registerResponseListener( NgHTTP2ResponseListener * listener );

                void createResponseEvent( struct HTTP2HandlerResponse * response );

                void createResponseHeaderStatusEvent( const char * status_code );

             private:

                 NgHTTP2ResponseListenerList m_response_listener;
        };
    }
}
#endif // NGHTTP2RESPONSEMANAGER_H
