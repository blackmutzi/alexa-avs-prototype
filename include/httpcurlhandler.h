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
#ifndef HTTPCURLHANDLER_H
#define HTTPCURLHANDLER_H

#include "logger.h"
#include "network.h"

namespace Network
{
    namespace HTTP
    {

        struct HTTPResponseMemory
        {
            char * memory;
            size_t size;
        };

        //@Abstract Class
        class HTTPCurlHandler
        {
            public:
                HTTPCurlHandler();
                virtual ~HTTPCurlHandler(){}

                virtual void init();

                void setResponseCallback( HTTPResponseMemory * chunk  ) throw (std::exception );

                CURL * getHandle(void);

                virtual void doIt(void) throw( std::exception );

                virtual void clean();

            protected:

                CURL * m_curl_handle;

                HTTPResponseMemory * m_response_content;

                static size_t WriteMemoryCallback( void * contents , size_t size , size_t nmemb , void *userp);


        };

    }
}

#endif // HTTPCURLHANDLER_H
