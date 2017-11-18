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
#ifndef NGHTTP2HANDLER_H
#define NGHTTP2HANDLER_H

#include "network.h"
#include "logger.h"
#include "nghttp2api.h"
#include "nghttp2responsemanager.h"

namespace Network
{
    namespace HTTP2
    {
        struct HTTP2HandlerSession
        {
            struct event_base * evbase;
            SSL_CTX * ssl_ctx;
            const char * host;
            uint16_t port;
            http2_session_data * session_data;

        };

        struct HTTP2HandlerResponse
        {
            char * memory;
            size_t size;

        };

        struct HTTP2HandlerResponseManagement
        {
            NgHTTP2ResponseManager * manager;
            HTTP2HandlerResponse   * response_memory;

        };

        //@Interface
        class NgHTTP2ResponseListener
        {
            public:
                virtual ~NgHTTP2ResponseListener(){}

                virtual void onResponseEvent( struct HTTP2HandlerResponse * response )=0;

                virtual void onResponseHeaderStatusEvent( const char * status_code ) =0;
        };

        using NgHTTP2Header = std::vector< nghttp2_nv * >;

        class NgHTTP2Handler : public NgHTTP2API
        {
             public:

                NgHTTP2Handler();
                ~NgHTTP2Handler();

                /*!
                 * init http2 session
                 * \brief Initialize
                 */
                void initialize( HTTP2HandlerResponse * response , NgHTTP2ResponseManager * manager ) throw (std::exception);
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
                 * \brief setResponseManager
                 * \param response_manager
                 */
                void setResponseManager( NgHTTP2ResponseManager * response_manager );

                /*!
                 * Set HTTP2 Request Header
                 * \brief setHeader
                 */
                void setRequestHeader( nghttp2_nv * ptr , size_t size ) throw ( std::exception );
                /*!
                 * Set HTTP2 Request Body Content
                 * \brief setRequestMultipart
                 * \param fd : FileDescriptor
                 */
                void setRequestSource( const std::string source ) throw( std::exception );

                /*!
                 * send data to server
                 * \brief perform
                 * \return
                 */
                void perform(void) throw( std::exception );

                /*!
                 * destroy http2 session
                 * \brief CleanUp
                 */
                void cleanUp(void) throw (std::exception );

            protected:

                HTTP2HandlerSession    * m_session;
                HTTP2HandlerResponse   * m_session_response;
                HTTP2HandlerResponseManagement * m_session_response_management;

                NgHTTP2ResponseManager * m_response_manager;
                nghttp2_nv * m_header_copy;

                static void onResponseHeaderEvent( const void * name , size_t name_size , const void * value , size_t value_size , void * userdata );
                static void onResponseBodyEvent( void * content , size_t size , void * userdata );

            private:

                bool m_has_header;
                bool m_has_source;
                bool m_init;


        };
    }
}
#endif // NGHTTP2HANDLER_H
