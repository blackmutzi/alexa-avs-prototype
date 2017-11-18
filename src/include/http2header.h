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
#ifndef HTTP2HEADER_H
#define HTTP2HEADER_H

#include "network.h"

namespace Network
{
    namespace HTTP
    {
        /*!
         * \brief HTTP/2 Message Headers
         *
         *  :method = {{verb}}
         *  :scheme = https
         *  :path = /{{API version}}/{{events or directives}}
         *  authorization = Bearer {{access_token}}
         *  content-type = multipart/form-data; boundary={{boundary_term}}
         *
         */
        class HTTP2Header
        {
            public:

               HTTP2Header();
               HTTP2Header(const std::string access_token , const std::string event_path );
               ~HTTP2Header();

               void setMethode(const std::string methode);
               void setSheme(const std::string scheme);
               void setPath( const std::string path );
               void setAuthorization( const std::string access_token );
               void setContentType( const std::string content_type );
               void setBoundary( const std::string boundary);

               const std::string & getMethode(void) const;
               const std::string & getSheme(void) const;
               const std::string & getPath(void) const;
               const std::string & getAccessToken(void) const;
               const std::string & getContentType(void) const;
               const std::string & getBoundary(void) const;

               static std::string HTTP2_HEADERMSG_METHOD;
               static std::string HTTP2_HEADERMSG_SHEME;
               static std::string HTTP2_HEADERMSG_PATH;
               static std::string HTTP2_HEADERMSG_AUTHORIZATION;
               static std::string HTTP2_HEADERMSG_CONTENT_TYPE;
               static std::string HTTP2_HEADERMSG_BOUNDARY;

               static std::string HTTP2_PING;
               static std::string HTTP2_EVENTS_ENDPOINT;
               static std::string HTTP2_DIRECTIVE_ENDPOINT;

            private:

               std::string m_methode;
               std::string m_sheme;
               std::string m_path;
               std::string m_access_token;
               std::string m_content_type;
               std::string m_boundary;
        };

    }
}

#endif // HTTP2HEADER_H
