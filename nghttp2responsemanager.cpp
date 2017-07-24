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
#include "include/nghttp2responsemanager.h"

using namespace Network::HTTP2;

NgHTTP2ResponseManager::NgHTTP2ResponseManager() {}

void NgHTTP2ResponseManager::registerResponseListener(NgHTTP2ResponseListener *listener)
{
    m_response_listener.push_back( listener );
}

void NgHTTP2ResponseManager::createResponseEvent( HTTP2HandlerResponse *response )
{
    if( m_response_listener.empty())
    {
        Logger::instance().info("Response Listener List is empty");
        Logger::instance().info("create Response Event is canceled");
        return;
    }

    for( NgHTTP2ResponseListener * listener : m_response_listener  )
    {
        listener->onResponseEvent( response );
    }
}

void NgHTTP2ResponseManager::createResponseHeaderStatusEvent( const char * status_code )
{
    for( NgHTTP2ResponseListener * listener : m_response_listener  )
    {
        listener->onResponseHeaderStatusEvent( status_code );
    }
}
