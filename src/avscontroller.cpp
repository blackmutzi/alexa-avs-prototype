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
#include "include/avscontroller.h"

int AVSController::S_WAKE_WORD_RELEASE_TRIES = 5;
int AVSController::S_WAKE_WORD_RELEASE_RETRY_DELAY_MS = 1000;
int AVSController::S_WAKE_WORD_PORT_NUMBER = 5123;


AVSController::AVSController(WakeWordIPCFactory *wakeWordIPCFactory, WakeWordDetectedHandler *wakeWordDetectedHandler) throw( std::exception )
    : WakeWordDetectedHandler() ,
      DirectivenListener() ,
      AVSClientListener()  ,

      m_wakeword_detected_handler(wakeWordDetectedHandler) ,
      m_wakeword_ipc(nullptr) ,
      m_wakeword_factory( wakeWordIPCFactory ) ,
      m_avs_client( nullptr ) ,
      m_avs_authenticator( nullptr ) ,
      m_avs_playback( new AVSPlayback() ) ,

      m_avs_playback_controller( new AVSPlaybackController( m_avs_client ) ) ,

      m_wakeword_agent_enabled(true) ,
      m_accept_wakeword_events(true) ,
      m_audio_streaming(false)
{
    // Read Configuration

    m_avs_authenticator = new AVSConfigAuthenticator("/etc/alexa/config.json");

    m_avs_authenticator = new AVSConfigAuthenticator("./config/config.json");

    m_avs_authenticator->readConfig();

    // Get Access Token
    std::string access_token = m_avs_authenticator->GenerateAccessToken( m_avs_authenticator->getRefreshToken() );

    m_avs_client = new AVSClient( "avs-alexa-eu.amazon.com" , access_token );

    // register avs_client_listener
    m_avs_client->addListener( this );

    // register listener
    m_avs_client->getEventManager()->registerDirectivenListener( this );


    // register audio listener ( audio directive )
    m_avs_client->getEventManager()->registerAudioDirectivenListener( m_avs_playback_controller );

    // init playbackThread ( AVS Audioplayer Feature )
    m_avs_playback_controller->initPlaybackThread();

}
AVSController::~AVSController(){}

void AVSController::startAVSClient()
{
    m_avs_client->start();
}

AVSClient * AVSController::getAVSClient(void)
{
    return m_avs_client;
}

AVSConfigAuthenticator * AVSController::getAuthenticator(void)
{
    return m_avs_authenticator;
}

AVSPlayback * AVSController::getAVSPlaybackHandle(void)
{
    return m_avs_playback;
}

void AVSController::startWakeWordServer(void)
{
    if( m_wakeword_agent_enabled )
    {
        try {

          Logger::instance().info("Creating Wake Word IPC | port Number:", S_WAKE_WORD_PORT_NUMBER );

          m_wakeword_ipc = m_wakeword_factory->createWakeWordIPC( this , S_WAKE_WORD_PORT_NUMBER  );
          m_wakeword_ipc->init();

          Logger::instance().info("Created Wake Word IPC ok.");

        }catch( ErrorException e )
        {
            Logger::instance().warn("Error creating Wake Word IPC ok.", e);
        }
    }
}

void AVSController::sendWakeWordSignal( IPCCommand command )
{
    try
    {
        m_wakeword_ipc->sendCommand( command );

    }catch( ErrorException e )
    {
        Logger::instance().warn("send wakeword signal failed ", e);
    }

}

void AVSController::startAudioStreaming(void)
{
    m_avs_client->pushStreamRequest( EVENT_STREAM_RECOGNICE );
}

void AVSController::stopAudioStreaming(void)
{
    Logger::instance().info("--- Stop Audio Streaming ---");
    m_audio_streaming = false;
}

void AVSController::onDeviceCaptureOpenEvent(void)
{
     sendWakeWordSignal( WakeWordIPC::IPC_PAUSE_WAKE_WORD_ENGINE );

     //@TODO Sound abspielen damit man weiß das jetzt eine Aufnahme statt findet!
}

void AVSController::onDeviceCaptureCloseEvent(void)
{
     sendWakeWordSignal( WakeWordIPC::IPC_RESUME_WAKE_WORD_ENGINE );
}

void AVSController::onStreamLoopStartEvent(void){}
void AVSController::onStreamLoopWaitEvent(void){}

void AVSController::onWakeWordDetected()
{
    if( m_accept_wakeword_events )
                m_wakeword_detected_handler->onWakeWordDetected();
}

void AVSController::onSpeekerSetMuteDirective( const HTTPClientDirectiveEvent * event )
{
    Logger::instance().info("--- Speak SetMute Directive Detected --- ");
    std::string payload_mute;
    JsonObject * object;
    // Analyse Json object
    try
    {
        object = event->getJsonObject(0);

        JsonReaderDirective * json_reader = new JsonReaderDirective();
        json_reader->setJsonContent( object->json_string );
        payload_mute = json_reader->getPayloadMute();
        delete json_reader;

        m_avs_client->getSyncState()->volumeState->muted = payload_mute;

        if( payload_mute == "true" )
        {
             Logger::instance().info("Playback is now muted");
             playback_mute_thread( getAVSPlaybackHandle() );
        }

        if( payload_mute == "false" )
        {
            Logger::instance().info("Playback is now unmuted");
            playback_unmute_thread( getAVSPlaybackHandle() );
        }

    }catch( std::exception e )
    {
        Logger::instance().warn("Payload Mute Token not found");
    }
}

void AVSController::onSpeechRecognizeStopCaptureDirective(const HTTPClientDirectiveEvent *event)
{
    (void) event;

    Logger::instance().info("--- Stop Capture Directive Detected --- ");

    stopAudioStreaming();
}

void AVSController::onSpeechRecognizeExpectSpeechDirective(const HTTPClientDirectiveEvent *event)
{
    (void) event;

    Logger::instance().info("--- Expect Speech Directive Detected --- ");

    // start audio capture & sending to avs
    startAudioStreaming();
}

void AVSController::onSpeechSynthesizerSpeakDirective(const HTTPClientDirectiveEvent *event)
{
   Logger::instance().info("--- Speak Directive Detected --- ");

   std::string speak_directive_payload_token;
   JsonObject * object;
   char * mp3_old_data = nullptr;
   size_t mp3_size = 0;

   // Analyse Json object
   try
   {
       object = event->getJsonObject(1);

       JsonReaderDirective * json_reader = new JsonReaderDirective();
       json_reader->setJsonContent( object->json_string );
       speak_directive_payload_token = json_reader->getPayloadToken();
       delete json_reader;

       m_avs_client->getSyncState()->speechState->token = speak_directive_payload_token;

       Logger::instance().info("Speak-Directive-Payload-Token:");
       Logger::instance().info( speak_directive_payload_token );

   }catch( std::exception e )
   {
       Logger::instance().warn("Speak-Directive Payload Token not found ");
   }


   mp3_old_data = event->getAudioObject(0)->memory;
   mp3_size     = event->getAudioObject(0)->size;

   char * mp3_copy_data  = new char[ mp3_size ];
   memcpy( mp3_copy_data , mp3_old_data , mp3_size );

   boost::thread playback_thread( &AVSController::playback_thread , this , mp3_copy_data , mp3_size );
   m_avs_client->pushStreamRequest( EVENT_STREAM_SPEECH_SYNTHESIZER_START );
}

void AVSController::playback_thread( AVSController * controller , char * mp3data , size_t mp3size )
{
    controller->getAVSPlaybackHandle()->mp3Playback( mp3data , mp3size );
    delete[] mp3data; // delete mp3_copy_data
    controller->getAVSClient()->pushStreamRequest( EVENT_STREAM_SPEECH_SYNTHESIZER_FINISH );
}

void AVSController::playback_mute_thread( AVSPlayback * playback_handle )
{
    playback_handle->mute();
}

void AVSController::playback_unmute_thread(AVSPlayback *playback_handle)
{
    playback_handle->unmute();
}
