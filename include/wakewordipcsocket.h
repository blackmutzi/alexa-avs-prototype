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
#ifndef WAKEWORDIPCSOCKET_H
#define WAKEWORDIPCSOCKET_H

#include "wakeword.h"
#include "wakewordipc.h"
#include "wakeworddetectedhandler.h"
#include "wakewordipcconnectedclient.h"

#include "thread.h"
#include "Runnable.h"

#include "network.h"
#include "serversocket.h"

namespace WakeWord
{

    class WakeWordIPCSocket : public WakeWordIPC , Thread::Runnable
    {
        public:

            static Logger & log;

            WakeWordIPCSocket( WakeWordDetectedHandler * handler , int portNumber ) throw( std::exception );
            ~WakeWordIPCSocket(){}

            //@WakeWordIPC::init
            void init();

            //@WakeWordIPC::sendCommand
            void sendCommand(IPCCommand) throw( std::exception );

            //@Thread::Runnable::run
            void run();

            //@Synchronized
            void registerClient( WakeWordIPCConnectedClient * new_client );

            //@Synchronized
            void unregisterClient( WakeWordIPCConnectedClient * old_client );

            //WakeWord Detected ...
            void processWakeWordDetected();

         private:

             Network::NetSocket::ServerSocket * m_serverSocket;

             std::list< WakeWordIPCConnectedClient * > m_connectedClients;


    };

}
#endif // WAKEWORDIPCSOCKET_H
