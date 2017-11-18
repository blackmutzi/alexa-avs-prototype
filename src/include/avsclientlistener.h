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
#ifndef AVSCLIENTLISTENER_H
#define AVSCLIENTLISTENER_H

#include "stdafx.h"

//@Interface
class AVSClientListener
{
    public:
        virtual ~AVSClientListener(){}

        virtual void onStreamLoopStartEvent(void) = 0;

        virtual void onStreamLoopWaitEvent(void) = 0;

        virtual void onDeviceCaptureOpenEvent(void)=0;

        virtual void onDeviceCaptureCloseEvent(void)=0;
};

#endif // STREAMLOOPLISTENER_H
