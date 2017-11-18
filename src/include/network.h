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
#ifndef NETWORK_H
#define NETWORK_H

#include "stdafx.h"

namespace Network
{
    namespace NetSocket
    {
          class ServerSocket;
    }

    namespace HTTP
    {
         class HTTPCurlHandler;

         class HTTPClient;
         class HTTPClientCommand;

         class HTTPRequest;
         class HTTPResponse;

         class HTTP2Header;
         class HTTPHeader;
         class HTTPContent;

         class HTTPMultiHeader;
         class HTTPMultiContent;

         class HTTPRequestFactory;
         class HTTPHeaderFactory;
         class HTTPContentFactory;

         class HTTPClientResponseListener;

         class HTTPClientDirectiveEvent;
         class HTTPClientRecognizeDirective;

         typedef struct
         {
             HTTPHeader  * header_one;
             HTTPHeader  * header_two;

             HTTPContent * content_one;
             HTTPContent * content_two;

         } HTTPRequestMultipartContent;
    }

    namespace HTTP2
    {
         class NgHTTP2ResponseListener;
         class NgHTTP2API;
         class NgHTTP2Handler;
         class NgHTTP2MultiplexHandler;
         class NgHTTP2ResponseManager;
         class HTTP2Request;

         struct HTTP2HandlerSession;
         struct HTTP2HandlerResponse;
         struct HTTP2HandlerResponseManagement;
    }
}


#endif // NETWORK_H
