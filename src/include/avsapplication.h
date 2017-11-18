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
#ifndef AVSAPPLICATION_H
#define AVSAPPLICATION_H

#include "stdafx.h"
#include "wakeword.h"
#include "wakeworddetectedhandler.h"
#include "avscontroller.h"
#include "avsclient.h"

using namespace WakeWord;

class AVSApplication : public WakeWordDetectedHandler
{
    public:
        AVSApplication();
        ~AVSApplication();

        void main(int argc, char **argv);

        void onWakeWordDetected(void);

    private:

        WakeWordIPCFactory * m_ipc_factory;
        AVSController      * m_avs_controller;
        AVSClient          * m_avs_client;

        bool m_started;
};

#endif // AVSAPPLICATION_H
