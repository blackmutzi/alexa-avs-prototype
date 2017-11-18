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
#ifndef DIRECTIVENLISTENER_H
#define DIRECTIVENLISTENER_H

#include "stdafx.h"
#include "network.h"
#include "httpclientdirectiveevent.h"

namespace Network
{
    namespace HTTP
    {
        //@Interface
        class DirectivenListener
        {
                public:
                   virtual ~DirectivenListener(){}

                   virtual void onSpeechSynthesizerSpeakDirective( const HTTPClientDirectiveEvent * event ) =0;

                   virtual void onSpeechRecognizeStopCaptureDirective( const HTTPClientDirectiveEvent * event ) =0;

                   virtual void onSpeechRecognizeExpectSpeechDirective( const HTTPClientDirectiveEvent * event )=0;

                   virtual void onSpeekerSetMuteDirective( const HTTPClientDirectiveEvent * event ) =0;
        };

        //@Interface
        class AudioPlayerDirectivenListener
        {
             public:
                virtual ~AudioPlayerDirectivenListener(){}

                virtual void onPlayDirective( const HTTPClientDirectiveEvent * event )=0;

                virtual void onStopDirective( const HTTPClientDirectiveEvent * event )=0;

                virtual void onClearQueueDirective( const HTTPClientDirectiveEvent * event )=0;
        };
    }
}

#endif // DIRECTIVENLISTENER_H
