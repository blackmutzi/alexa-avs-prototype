#ifndef AVSPLAYBACKCONTROLLER_H
#define AVSPLAYBACKCONTROLLER_H

#include "stdafx.h"
#include "logger.h"
#include "audio.h"
#include "directivenlistener.h"
#include "avsclient.h"
#include "avsplayback.h"

using namespace Audio;
using namespace Network::HTTP;

struct PlaybackQueue
{
    char * mp3_data;
    size_t mp3_size;

    std::string token;
    long offsetInMilliseconds;
};

class AVSPlaybackController : public AudioPlayerDirectivenListener
{
    public:
        AVSPlaybackController( AVSClient * avs_client );
        ~AVSPlaybackController();

        void initPlaybackThread(void);

        void pushPlaybackQueue( PlaybackQueue queue );

        void clearPlaybackQueue(void);

        void playmusic( PlaybackQueue queue );

        void stopmusic(void);

        void pausemusic(void);

        void resumemusic(void);

        bool isThreadRunning(void);

        bool isPlaybackStopped(void);

        void setPlaybackThreadWaiting( bool status );

        void setPlaybackRunning( bool status );

        AVSClient * getAVSClient(void);

        void onPlayDirective(const HTTPClientDirectiveEvent *event);

        void onStopDirective(const HTTPClientDirectiveEvent *event);

        void onClearQueueDirective(const HTTPClientDirectiveEvent *event);

    protected:

            void onPlaybackThreadProcess(void);

            static void playback_thread( AVSPlaybackController * controller );

    private:

        AVSClient * m_avs_client;
        AVSPlayback * m_avs_playback;

        bool m_playback_thread_running;
        bool m_playback_thread_waiting;
        bool m_playback_paused;
        bool m_playback_running;
        bool m_playback_stopped;

        boost::shared_ptr< std::queue < PlaybackQueue > > m_playback_queue;
        boost::mutex m_mutex;
        boost::condition_variable m_cv;
};

#endif // AVSPLAYBACKCONTROLLER_H
