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
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "network.h"
#include "httpcurlhandler.h"
#include "httpresponse.h"
#include "http2header.h"
#include "httpheader.h"
#include "httpcontent.h"

namespace Network
{
    namespace HTTP
    {
        class HTTPRequest : public HTTPCurlHandler
        {
            public:
                HTTPRequest( const std::string & url, HTTPResponse * response );
                ~HTTPRequest();

                void addHeaderData( std::string header_data );

                void writeHeaderData();

                void setPostData( std::string data );

                void clean(void) final;

                void doIt(void) throw(std::exception) final;

            private:

                HTTPResponse * m_response;
                HTTPResponseMemory * m_response_memory;
                std::string m_base_url;
                struct curl_slist * m_header_content;
        };
    }
}

#endif // HTTPREQUEST_H
