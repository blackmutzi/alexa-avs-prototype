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
#include "include/avsclient.h"

AVSClient::AVSClient( const std::string url , const std::string access_token)
    : HTTP2Client( url ) ,
      m_avs_request_builder( new AVSRequestBuilder( (HTTP2Client*) this , access_token , "-----abcde12345789" )) ,
      m_sync_event( new AVSJson::SynchronizeStateEvent() ) ,
      m_base_url( url ) ,
      m_active( false )
{
    //pre fill sync state
    m_sync_event->alertsState    = new AVSJson::AlertsState();
    m_sync_event->playbackState  = new AVSJson::PlaybackState();
    m_sync_event->speechState    = new AVSJson::SpeechState();
    m_sync_event->volumeState    = new AVSJson::VolumeState();
    m_sync_event->recognizeState = new AVSJson::RecognizeState();

    //Generate UUIDs
    boost::uuids::uuid message_id_uuid = boost::uuids::uuid( boost::uuids::random_generator()() );
    m_sync_event->messageId = boost::lexical_cast<std::string>( message_id_uuid );

    //AlertsState
    m_sync_event->alertsState->token = "";//boost::lexical_cast<std::string>( alert_token );
    m_sync_event->alertsState->type  = AVSJson::Alerts::ALERTS_TYPE_TIMER;
    m_sync_event->alertsState->scheduledTime = "";

    //PlaybackState
    m_sync_event->playbackState->token = "";//boost::lexical_cast<std::string>( playback_token );
    m_sync_event->playbackState->playerActivity = AVSJson::AudioPlayer::PLAYER_ACTIVITY_IDLE;
    m_sync_event->playbackState->offsetInMilliseconds = 0;

    //Alexa SpeechSynthesizer States
    m_sync_event->speechState->token = "";//boost::lexical_cast<std::string>( speech_token );
    m_sync_event->speechState->playerActivity = AVSJson::SpeechSynthesizer::PLAYER_ACTIVITY_FINISHED;
    m_sync_event->speechState->offsetInMilliseconds = 0;

    //VolumeState
    m_sync_event->volumeState->volume = 0;
    m_sync_event->volumeState->muted  = "false";

    //RecognizeState
    m_sync_event->recognizeState->wakeword = AVSJson::SpeechRecognizer::PAYLOAD_WAKEWORD_ALEXA;
}

AVSClient::~AVSClient()
{
   delete m_avs_request_builder;
   delete m_sync_event;
}

void AVSClient::addListener(AVSClientListener *listener)
{
    m_avs_client_listener_list.push_back( listener );
}

void AVSClient::removeListener(AVSClientListener *listener)
{
    m_avs_client_listener_list.remove( listener );
}

AVSRequestBuilder * AVSClient::getRequestBuilder(void)
{
    return m_avs_request_builder;
}

AVSJson::SynchronizeStateEvent * AVSClient::getSyncState(void)
{
    return m_sync_event;
}

void AVSClient::start(void)
{
    try{

       m_active = true;

       boost::thread thread_downchannel( &AVSClient::downchannel_process , this );

       while( *getEventManager()->getStreamStatus() != STREAM_LOOP_HAS_STARTED ){}

           //boost::thread thread_ping( &AVSClient::ping_process , this );

           boost::this_thread::sleep(boost::posix_time::seconds(5));
           pushStreamRequest( EVENT_STREAM_SYNCSTATE );
           pushStreamRequest( EVENT_STREAM_SETTINGS_UPDATE );


    }catch( ErrorException e )
    {
        Logger::instance().warn("avs-client failed");
        Logger::instance().warn( e.what() );
    }
}

void AVSClient::downchannel_process( AVSClient *client )
{
    if( client->isActive() )
    {
        client->getRequestBuilder()->invokeDownchannelStream();
    }
}

void AVSClient::ping_process(AVSClient *client)
{
    while( client->isActive() )
    {
        //@ Ping Request every 5 minutes (300 seconds)
        boost::this_thread::sleep(boost::posix_time::seconds(300));
        client->pushStreamRequest( EVENT_STREAM_PING );
    }
}

void AVSClient::onStreamLoopStartEvent(void)
{
    for( AVSClientListener * listener : m_avs_client_listener_list )
    {
        listener->onStreamLoopStartEvent();
    }
}

void AVSClient::onStreamLoopWaitEvent(void)
{
     Logger::instance().info("--- Stream Loop Entered Wait Mode ---");

     for( AVSClientListener * listener : m_avs_client_listener_list )
     {
         listener->onStreamLoopWaitEvent();
     }
}

StreamQueueData * AVSClient::onProcessStreamEvent( StreamQueueEvent event )
{
    StreamQueueData * data;
    AudioCapture capture;
    snd_pcm_uframes_t frames = 0;
    char * audio_bytes;
    AVSJson::JsonFactory factory;
    std::string message_id = "";

    boost::uuids::uuid message_id_uuid = boost::uuids::uuid( boost::uuids::random_generator()() );
    message_id = boost::lexical_cast<std::string>( message_id_uuid );


    switch( event )
    {
       case EVENT_STREAM_SYNCSTATE:
               data = getRequestBuilder()->buildSyncStream( m_sync_event );
       break;

       case EVENT_STREAM_PING:
               data = getRequestBuilder()->buildPingStream();
       break;

       case EVENT_STREAM_RECOGNICE:

               for( AVSClientListener * listener : m_avs_client_listener_list )
               {
                    listener->onDeviceCaptureOpenEvent();
               }

               capture.openDevice("default");

               audio_bytes = (char*) capture.captureLoop( frames );

               capture.closeDevice();

               for( AVSClientListener * listener : m_avs_client_listener_list )
               {
                   listener->onDeviceCaptureCloseEvent();
               }

               data = getRequestBuilder()->buildRecognizeStream( m_sync_event , "request-id-1000103" , audio_bytes , frames * 2 );

               free( audio_bytes );
       break;

       case EVENT_STREAM_SETTINGS_UPDATE:
               data = getRequestBuilder()->buildEventStream( factory.createSettingsUpdateEvent( message_id , "locale", "de-DE") );
       break;

       case EVENT_STREAM_SPEECH_SYNTHESIZER_START:

               data = getRequestBuilder()->buildEventStream( factory.createSpeechSynthesizerStartEvent( message_id , m_sync_event->speechState->token ) );
       break;

       case EVENT_STREAM_SPEECH_SYNTHESIZER_FINISH:

               data = getRequestBuilder()->buildEventStream( factory.createSpeechSynthesizerFinishEvent( message_id , m_sync_event->speechState->token ) );
       break;
    }

    return data;
}

bool AVSClient::isActive(void)
{
    return m_active;
}

void AVSClient::changeAccessToken( const std::string access_token )
{
   m_avs_request_builder->changeAccessToken( access_token );
}

void AVSClient::restart( void )
{
    m_active = false;
    start();
}

