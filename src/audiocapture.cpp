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
#include "include/audiocapture.h"

using namespace Audio;

AudioCapture::AudioCapture()
    : m_isDeviceOpen( false ) , m_isCaptureStarted( false )
{}

AudioCapture::~AudioCapture()
{}

void AudioCapture::timer( int * flags )
{
    Logger::instance().info("Timer is running ...");
    boost::this_thread::sleep(boost::posix_time::seconds(5));
    *flags = 0;
    Logger::instance().info("Timer has stopped.");
}


const char * AudioCapture::captureLoop( snd_pcm_uframes_t & all_frames )
{
    int flags = 1;

    snd_pcm_uframes_t buffer_frames = 1024;

    size_t buffer_size;

    char * cap_audio_bytes;

    /* audio format - Signed 16 bit Little Endian ( 2 Bytes / chars  )*/
    AUDIO_PCM_FORMAT format = SND_PCM_FORMAT_S16_LE;

    int bit_width = snd_pcm_format_width( format ); // are 16 Bits

    int channel   = 1 ;

    int bytes     = bit_width / (8 * channel);       // are 2 Bytes

    /* audio buffer size in byte size : frame * (char * 2) ( aka. bytes )  */
    buffer_size = buffer_frames * bytes;

    /* allocate a byte array */
    cap_audio_bytes = ( char * ) malloc( sizeof( char ) * buffer_size );

    int pcmreturn;
    char * byte_stream = (char*) malloc( sizeof( char ) * buffer_size );
    unsigned int while_counter = 0;

    //time process
    boost::thread thread( &AudioCapture::timer , &flags );
    Logger::instance().info("Start capturing ...");

    while( flags )
    {
        pcmreturn = snd_pcm_readi( capture_handle , cap_audio_bytes , buffer_frames );

        if (pcmreturn == -EPIPE)
        {
           /* EPIPE means overrun */
           snd_pcm_prepare( capture_handle );
        }

        if( pcmreturn != buffer_frames )
        {
            Logger::instance().info("short read detected:", pcmreturn );
        }

        if( pcmreturn == buffer_frames )
        {
            if( while_counter != 0 )
                    byte_stream = (char*) realloc( byte_stream , sizeof( char ) * (buffer_size * (while_counter + 1)) );

            memcpy( byte_stream + (buffer_size * while_counter) , cap_audio_bytes , buffer_size );

            all_frames += buffer_frames;
            while_counter++;
        }
    }

    Logger::instance().info("Stop capturing ...");

    free( cap_audio_bytes );

    return byte_stream;
}


void AudioCapture::closeDevice(void)
{
    snd_pcm_drain(capture_handle);
    snd_pcm_close(capture_handle);

    m_isDeviceOpen = false;
}

AUDIO_PCM_HANDLE * AudioCapture::openDevice( const char * device ) throw( std::exception )
{
    if( m_isDeviceOpen )
          throw ErrorException("Capture Device is already open:", device);

    int error = 0;
    unsigned int rate = 16000;//44100;
    unsigned int channels = 1;
    AUDIO_PCM_FORMAT format = SND_PCM_FORMAT_S16_LE;

    if( (error = snd_pcm_open( &capture_handle , device , SND_PCM_STREAM_CAPTURE, 0)) < 0)
    {
        throw ErrorException("cannot open audio device:", snd_strerror(error) );
    }

    Logger::instance().info("Audio interface is open ... ");

    if( (error = snd_pcm_hw_params_malloc( &hw_params)) < 0 )
    {
        throw ErrorException("cannot allocate hardware parameter structure:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params allocated ...");

    if( (error = snd_pcm_hw_params_any ( capture_handle , hw_params)) < 0)
    {
        throw ErrorException("cannot initialize hardware aprameter structure:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params initialized");

    if ((error = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        throw ErrorException("cannot set access type:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params access setted");

    if( (error = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0)
    {
        throw ErrorException("cannot set sample format:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - format setted");


    if( (error = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0)
    {
        throw ErrorException("cannot set sample rate", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - rate setted ");

    if ((error = snd_pcm_hw_params_set_channels (capture_handle, hw_params, channels )) < 0)
    {
        throw ErrorException("cannot set channel count", snd_strerror(error) );
    }

    /* Set period size to 1024 frames. */
    snd_pcm_uframes_t frames = 1024;
    int dir;
    snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir);

    Logger::instance().info("Audio HW Params - channels setted");

    if ((error = snd_pcm_hw_params (capture_handle, hw_params)) < 0)
    {
        throw ErrorException("cannot set parameters", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - setted");
    snd_pcm_hw_params_free (hw_params);

    if ((error = snd_pcm_prepare (capture_handle)) < 0) {

        throw ErrorException("cannot prepare audio interface for use ", snd_strerror(error) );
    }

    m_isDeviceOpen = true;
    Logger::instance().info("Audio Device is now open");

    return capture_handle;
}

