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
#ifndef NGHTTP2MULTIPLEXHANDLER_H
#define NGHTTP2MULTIPLEXHANDLER_H

#include "network.h"
#include "logger.h"
#include "nghttp2api.h"
#include "nghttp2handler.h"
#include "nghttp2responsemanager.h"

namespace Network
{
    namespace HTTP2
    {
        enum HandlerStatus
        {
            STREAM_LOOP_HAS_STARTED ,
            STREAM_SESSION_TERMINATED
        };

        struct StreamQueueData
        {
            StreamQueueData()
            {
                header_data = nullptr;
                source_data = nullptr;
                header_len  = 0;
                source_len  = 0;
            }

            ~StreamQueueData()
            {
                if( source_data != nullptr )
                {
                    delete[] source_data;
                    source_data = nullptr;
                    source_len = 0;
                }

                if( header_data != nullptr )
                {
                    for( unsigned int count =0; count < header_len; count++ )
                    {
                        delete[] header_data[ count ].value;
                        header_data[ count ].valuelen = 0;
                    }

                    delete[] header_data;
                    header_len = 0;
                }
            }

            /* http2_header_data*/
            nghttp2_nv * header_data;
            /* http2_source_data */
            char* source_data;
            /* http2_header_len */
            size_t header_len;
            /* http2_source_len*/
            size_t source_len;
        };

        class NgHTTP2MultiplexHandler : public NgHTTP2API
        {
             public:

                NgHTTP2MultiplexHandler();
                virtual ~NgHTTP2MultiplexHandler();

                /*!
                 * init http2 session
                 * \brief Initialize
                 */
                void initialize( HTTP2HandlerResponse * response , NgHTTP2ResponseManager * manager ) throw (std::exception);

                void initiateStream( const char * uri , nghttp2_nv * header_data , size_t arr_len , int * status );

                void createNewStream( StreamQueueData * data , struct event_base * evbase );

                /*!
                 * Set URI : https://example.de
                 * without https only example.de
                 * \brief setUri
                 * \param uri
                 */
                void setUri( const char * uri ) throw ( std::exception );

                /*!
                 * Set Server Port
                 * \brief setPort
                 * \param port
                 * Default: 443
                 */
                void setPort( uint16_t port ) throw ( std::exception );

                /*!
                 * destroy http2 session
                 * \brief CleanUp
                 */
                void cleanUp(void) throw (std::exception );

                /*!
                 * override it ...
                 * \brief stream_loop
                 * \param handler
                 */
                virtual void stream_loop( struct event_base * evbase ) =0;

            protected:

                HTTP2HandlerSession    * m_session;
                HTTP2HandlerResponseManagement * m_session_response_management;

                static void onResponseHeaderEvent( const void * name , size_t name_size , const void * value , size_t value_size , void * userdata );
                static void onResponseBodyEvent( void * content , size_t size , void * userdata );

            private:

                bool m_init;

        };
    }
}

#endif // NGHTTP2MULTIPLEXHANDLER_H
