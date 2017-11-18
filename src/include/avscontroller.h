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
#ifndef AVSCONTROLLER_H
#define AVSCONTROLLER_H

#include "stdafx.h"
#include "logger.h"

#include "wakeword.h"
#include "wakeworddetectedhandler.h"
#include "wakewordipcfactory.h"
#include "wakewordipc.h"

#include "audio.h"
#include "network.h"
#include "audiocapture.h"
#include "audioplayback.h"
#include "avsclient.h"

#include "avsauthenticator.h"
#include "avsconfigauthenticator.h"
#include "avsplayback.h"
#include "directivenlistener.h"
#include "avsplaybackcontroller.h"


using namespace WakeWord;
using namespace Audio;
using namespace Network::HTTP;

class AVSController : public WakeWordDetectedHandler , public DirectivenListener , public AVSClientListener
{
    public:

        AVSController( WakeWordIPCFactory * wakeWordIPCFactory , WakeWordDetectedHandler * wakeWordDetectedHandler ) throw ( std::exception );
        ~AVSController();

        void startAVSClient(void);
        void startWakeWordServer(void);
        void sendWakeWordSignal( IPCCommand command );
        void startAudioStreaming(void);
        void stopAudioStreaming(void);

        AVSClient * getAVSClient(void);
        AVSConfigAuthenticator * getAuthenticator(void);
        AVSPlayback * getAVSPlaybackHandle(void);

        static void playback_thread( AVSController * controller , char * mp3data , size_t mp3size );
        static void playback_mute_thread( AVSPlayback * playback_handle );
        static void playback_unmute_thread( AVSPlayback * playback_handle );

    protected:

        //@AVSClientListener Interface
        void onStreamLoopStartEvent(void);
        void onStreamLoopWaitEvent(void);
        void onDeviceCaptureOpenEvent(void);
        void onDeviceCaptureCloseEvent(void);

        //@WakeWordDetectedHandler Interface
        void onWakeWordDetected();

        //@DirectivenListener
        void onSpeechRecognizeStopCaptureDirective ( const HTTPClientDirectiveEvent * event );
        void onSpeechRecognizeExpectSpeechDirective( const HTTPClientDirectiveEvent * event );
        void onSpeechSynthesizerSpeakDirective( const HTTPClientDirectiveEvent * event );
        void onSpeekerSetMuteDirective(const HTTPClientDirectiveEvent *event);

    private:

        static int S_WAKE_WORD_RELEASE_TRIES;
        static int S_WAKE_WORD_RELEASE_RETRY_DELAY_MS;
        static int S_WAKE_WORD_PORT_NUMBER;

        WakeWordDetectedHandler * m_wakeword_detected_handler;
        WakeWordIPC * m_wakeword_ipc;
        WakeWordIPCFactory * m_wakeword_factory;
        AVSClient          * m_avs_client;
        AVSConfigAuthenticator * m_avs_authenticator;
        AVSPlayback * m_avs_playback;
        AVSPlaybackController * m_avs_playback_controller;


        bool m_wakeword_agent_enabled;
        bool m_accept_wakeword_events;
        bool m_audio_streaming;

};

#endif // AVSCONTROLLER_H
