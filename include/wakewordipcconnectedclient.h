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
#ifndef WAKEWORDIPCCONNECTEDCLIENT_H
#define WAKEWORDIPCCONNECTEDCLIENT_H

#include "logger.h"

#include "wakeword.h"
#include "wakewordipcsocket.h"
#include "wakewordipc.h"


#include "thread.h"
#include "Runnable.h"

namespace WakeWord
{
    using IPCCommand = WakeWordIPC::IPCCommand;

    class WakeWordIPCConnectedClient : public Thread::Runnable
    {
        public:

            WakeWordIPCConnectedClient( int client_socket , WakeWordIPCSocket * ipc_socket );
            ~WakeWordIPCConnectedClient(){}

            void init();

            //@Thread::Runnable::run
            void run();

            void terminate();

            void sendCommand( IPCCommand command ) throw( std::exception );

        private:

         WakeWordIPCSocket * m_wakeword_ipc_socket;
         int m_client_socket;
         bool m_running;

    };
}

#endif // WAKEWORDIPCCONNECTEDCLIENT_H
