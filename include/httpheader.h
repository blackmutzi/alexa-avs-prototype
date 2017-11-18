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
#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include "network.h"
#include "httpcontent.h"

namespace Network
{
    namespace HTTP
    {
        /*!
         * \brief The HTTPHeader class
         *
         * JSON Header:
         * Content-Disposition: form-data; name="metadata"
         * Content-Type: application/json; charset=UTF-8
         *
         * Binary Audio Header:
         * Content-Disposition: form-data; name="audio"
         * Content-Type: application/octet-stream
         *
         */
        class HTTPHeader
        {
            public:
                HTTPHeader(const std::string boundary );
                HTTPHeader(const std::string content_disposition , const std::string content_tpye );
                ~HTTPHeader(){}

                void setContentDisposition( const std::string content_disposition);
                void setContentType( const std::string content_type );
                void setBoundary( const std::string boundary );

                const std::string & getContentDisposition(void) const;
                const std::string & getContentType(void) const;
                const std::string & getBoundary(void) const;

                static std::string HTTP_HEADERMSG_CONTENT_DISPOSITION;
                static std::string HTTP_HEADERMSG_CONTENT_TYPE;

            private:

                std::string m_content_disposition;
                std::string m_content_type;
                std::string m_boundary;


        };
    }
}


#endif // HTTPHEADER_H
