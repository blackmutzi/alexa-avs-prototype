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
#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "network.h"
#include "logger.h"

namespace Network
{
    namespace NetSocket
    {
        class ServerSocket
        {
             public:

                ServerSocket( int portNumber );
                ~ServerSocket();

                int socketAccept() throw( std::exception );

                void destroy_socket( int client_socket );

                void destroy_server();

             private:

                int m_server_socket;
        };
    }

}
#endif // SERVERSOCKET_H
