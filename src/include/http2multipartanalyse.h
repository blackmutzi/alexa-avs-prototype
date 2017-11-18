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
#ifndef HTTP2MULTIPARTANALYSE_H
#define HTTP2MULTIPARTANALYSE_H

#include "stdafx.h"
#include "network.h"
#include "logger.h"
#include "httpclientdirectiveevent.h"

using namespace Network::HTTP;

namespace Network
{
    namespace HTTP2
    {
        typedef struct
        {
            char * bytes;
            size_t size;

        } DIRECTIVE_BYTES , AUDIO_BYTES ;

        class HTTP2MultipartAnalyse
        {
            public:
                HTTP2MultipartAnalyse();
                ~HTTP2MultipartAnalyse(){}

                void analyse( char * content , size_t content_size );
                void update( HTTPClientDirectiveEvent * event );

            protected:

                size_t nextEntryPoint( char * content , size_t offset , char delim );
                std::vector< size_t > getEntryPositions( char * content , size_t size , unsigned int amount , std::string needle , unsigned int * real_amount );
                size_t search( char * content , size_t offset , size_t size , std::string needle );
                void debug_write_file(const char * file , const char * content , size_t content_size );

            private:

                DIRECTIVE_BYTES * m_directive_bytes;
                AUDIO_BYTES * m_audio_bytes;
                size_t m_directive_allocate_count;
                size_t m_audio_allocate_count;

        };
    }
}

#endif // HTTP2MULTIPARTANALYSE_H
