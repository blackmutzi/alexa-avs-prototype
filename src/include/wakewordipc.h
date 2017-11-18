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
#ifndef WAKEWORDIPC_H
#define WAKEWORDIPC_H

#include "stdafx.h"
#include "wakeword.h"
#include "wakeworddetectedhandler.h"

namespace WakeWord
{
    //@Abstract Class
    class WakeWordIPC
    {
        public:

        enum IPCCommand : unsigned int  {

            IPC_DISCONNECT = 1 ,
            IPC_WAKE_WORD_DETECTED = 2 ,
            IPC_PAUSE_WAKE_WORD_ENGINE = 3,
            IPC_RESUME_WAKE_WORD_ENGINE = 4,
            IPC_CONFIRM = 5 ,
            UNKNOWN = 6
        };


        static const char * IPCCommandNames[];

        public:
            WakeWordIPC( WakeWordDetectedHandler * handler );
            virtual ~WakeWordIPC(){}

            //@Abstract Method
            virtual void sendCommand( IPCCommand ) throw( std::exception )=0;

            //@Abstract Method
            virtual void init()=0;

        protected:

            void wakeWordDetected();

        private:

            WakeWordDetectedHandler * m_wakeword_handler;

    };
}

#endif // WAKEWORDIPC_H
