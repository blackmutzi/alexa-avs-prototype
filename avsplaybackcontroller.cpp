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
#include "include/avsplaybackcontroller.h"

AVSPlaybackController::AVSPlaybackController( AVSClient * client )
    : AudioPlayerDirectivenListener() ,
      m_avs_client( client ) ,
      m_avs_playback( new AVSPlayback() ) ,
      m_playback_thread_running( false )  ,
      m_playback_thread_waiting( false )  ,
      m_playback_paused( false )          ,
      m_playback_running( false )         ,
      m_playback_stopped( false )
{
    m_playback_queue.reset( new std::queue < PlaybackQueue >() );
}

AVSPlaybackController::~AVSPlaybackController(){}

void AVSPlaybackController::initPlaybackThread(void)
{
    m_playback_thread_running = true;
    boost::thread playback_thread( &AVSPlaybackController::playback_thread , this );
}

void AVSPlaybackController::pushPlaybackQueue(PlaybackQueue queue)
{
    std::queue< PlaybackQueue > * origin_queue = m_playback_queue.get();
    origin_queue->push( queue );
    m_cv.notify_one();
}

void AVSPlaybackController::clearPlaybackQueue(void)
{
    std::queue < PlaybackQueue > * queue = m_playback_queue.get();

    while ( !queue->empty() )
    {
       queue->pop();
    }
}

bool AVSPlaybackController::isThreadRunning(void)
{
    return m_playback_thread_running;
}

void AVSPlaybackController::playmusic(PlaybackQueue queue)
{
    m_avs_playback->mp3Playback( queue.mp3_data , queue.mp3_size );
}

void AVSPlaybackController::pausemusic(void)
{
    m_playback_paused = true;
    m_avs_playback->mute();
    m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_PAUSED );
}

void AVSPlaybackController::resumemusic(void)
{
    m_playback_paused = false;
    m_avs_playback->unmute();
    m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_RESUME );
}

void AVSPlaybackController::stopmusic(void)
{
    m_playback_stopped = true;

    if( m_playback_thread_waiting )
            m_cv.notify_one();

    if( m_playback_paused )
              resumemusic();

    while( m_playback_running ){} // wait mode until m_playback_running is false

    m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_STOPPED );
}

bool AVSPlaybackController::isPlaybackStopped(void)
{
    return m_playback_stopped;
}

void AVSPlaybackController::setPlaybackThreadWaiting( bool status )
{
   m_playback_thread_waiting = status;
}

void AVSPlaybackController::setPlaybackRunning(bool status)
{
    m_playback_running = status;
}

AVSClient * AVSPlaybackController::getAVSClient(void)
{
    return m_avs_client;
}

void AVSPlaybackController::onPlayDirective(const HTTPClientDirectiveEvent *event)
{
    Logger::instance().info("--- Play Directive Detected ---");

    JsonObject * play_directive = event->getJsonObject(1);
    std::string playBehavior = "";
    std::string token = "";
    std::string offset = "";
    long offsetInMilliseconds = 0;

    try{

        json json_parser = json::parse( play_directive->json_string );
        playBehavior     = json_parser["directive"]["payload"]["playBehavior"];
        token            = json_parser["directive"]["payload"]["audioItem"]["stream"]["token"];
        offset           = json_parser["directive"]["payload"]["audioItem"]["stream"]["offsetInMilliseconds"];
        offsetInMilliseconds = std::stol( offset );

        if( playBehavior == "REPLACE_ALL")
        {
            stopmusic();
            clearPlaybackQueue();

            // push queue
            PlaybackQueue push_queue;
            push_queue.mp3_data = (char *) malloc( event->getAudioObject(0)->size );
            push_queue.mp3_size = event->getAudioObject(0)->size;
            memcpy( push_queue.mp3_data , event->getAudioObject(0)->memory , push_queue.mp3_size );
            push_queue.token    = token;
            push_queue.offsetInMilliseconds = offsetInMilliseconds;

            pushPlaybackQueue( push_queue );
            m_playback_stopped = false;
        }

        if( playBehavior == "ENQUEUE")
        {
            // Push to the End

            PlaybackQueue push_queue;
            push_queue.mp3_data = (char *) malloc( event->getAudioObject(0)->size );
            push_queue.mp3_size = event->getAudioObject(0)->size;
            memcpy( push_queue.mp3_data , event->getAudioObject(0)->memory , push_queue.mp3_size );
            push_queue.token    = token;
            push_queue.offsetInMilliseconds = offsetInMilliseconds;

            pushPlaybackQueue( push_queue );
            m_playback_stopped = false;
        }

        if( playBehavior == "REPLACE_ENQUEUED")
        {
            // clear queue
            clearPlaybackQueue();

            // push queue

            PlaybackQueue push_queue;
            push_queue.mp3_data = (char *) malloc( event->getAudioObject(0)->size );
            push_queue.mp3_size = event->getAudioObject(0)->size;
            memcpy( push_queue.mp3_data , event->getAudioObject(0)->memory , push_queue.mp3_size );
            push_queue.token    = token;
            push_queue.offsetInMilliseconds = offsetInMilliseconds;

            pushPlaybackQueue( push_queue );
            m_playback_stopped = false;
        }

    }catch( std::exception e )
    {
        Logger::instance().info("Bad Json Object or playBehavior not found ");
    }
}

void AVSPlaybackController::onStopDirective(const HTTPClientDirectiveEvent *event)
{
    Logger::instance().info("--- Stop Directive Detected ---");

    stopmusic();
}

void AVSPlaybackController::onClearQueueDirective(const HTTPClientDirectiveEvent *event)
{
    Logger::instance().info("--- ClearQueue Directive Detected ---");

    JsonObject * clear_queue_directive = event->getJsonObject(1);
    std::string  clearBehavior = "";

    try{

        json json_parser = json::parse( clear_queue_directive->json_string );
        clearBehavior    = json_parser["directive"]["payload"]["playBehavior"];

        if( clearBehavior == "CLEAR_ALL")
        {
            clearPlaybackQueue();

            stopmusic();
        }

        if( clearBehavior == "CLEAR_ENQUEUED")
        {
            clearPlaybackQueue();
        }


     }catch( std::exception e )
     {
        Logger::instance().info("Bad Json Object or clearBehavior not found ");
     }
}

void AVSPlaybackController::onPlaybackThreadProcess(void)
{
    boost::unique_lock< boost::mutex > lk( m_mutex );
    std::queue < PlaybackQueue > * queue = m_playback_queue.get();

    if( queue->empty() )
    {
        setPlaybackThreadWaiting( true );

        m_cv.wait( lk );

        setPlaybackThreadWaiting( false );

        if( isPlaybackStopped() )
                 return;
    }

    if( queue->empty() )
            return;

    PlaybackQueue playback_queue = queue->front();

    setPlaybackRunning( true );

        getAVSClient()->getSyncState()->playbackState->token = playback_queue.token;
        getAVSClient()->getSyncState()->playbackState->offsetInMilliseconds = playback_queue.offsetInMilliseconds;

        m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_STARTED );

        if( queue->size() <= 2 )
        {
              m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_NEARLY_FINISHED );
        }

        playmusic( playback_queue );

        free( playback_queue.mp3_data );

        queue->pop();

        m_avs_client->pushStreamRequest( EVENT_STREAM_PLAYBACK_FINISHED );

    setPlaybackRunning( false );
}

void AVSPlaybackController::playback_thread(AVSPlaybackController *controller)
{
    while( controller->isThreadRunning() )
    {
        while( !controller->isPlaybackStopped() )
        {
            controller->onPlaybackThreadProcess();
        } //@Playback::Loop
    } //@Thread::Loop
}













