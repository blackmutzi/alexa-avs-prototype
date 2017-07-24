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
#include "include/audioplayback.h"

using namespace Audio;

AudioPlayback::AudioPlayback(void)
    : m_bitrate( 16000 ) ,
      m_isDeviceOpen( false ) ,
      m_muted( false )
{}

AudioPlayback::AudioPlayback( unsigned int bitrate )
    : m_bitrate( bitrate ) ,
      m_isDeviceOpen( false ) ,
      m_muted( false )
{}

void AudioPlayback::setBitrate(unsigned int bitrate)
{
    m_bitrate = bitrate;
}

void AudioPlayback::setMutedStatus( bool status )
{
    m_muted = status;
}

void AudioPlayback::playback( char * audio_bytes , snd_pcm_uframes_t audio_frames , int bits_per_sample ) throw(std::exception)
{
    if( !m_isDeviceOpen )
            throw ErrorException("device is not open:");

    if( audio_bytes == nullptr )
            throw ErrorException("NullPointerException by audio_bytes ");

    int error = 0 , rc = 0 , loop_count = 0;
    unsigned int frame_rythmus   = 576;
    unsigned int pcm_buffer_size = frame_rythmus * bits_per_sample ;
    unsigned int offset          = 0;

    for( snd_pcm_uframes_t frames = frame_rythmus ; frames <= audio_frames; frames += frame_rythmus )
    {
            if( m_muted ) // if true then break - cancel audio output
            {
                break;
            }

            offset = pcm_buffer_size * loop_count;
            rc = snd_pcm_writei(playback_handle, audio_bytes + offset , frame_rythmus );

            if( rc == -EPIPE )
            {
                if ((error = snd_pcm_prepare(playback_handle) ) < 0)
                {
                    throw ErrorException("Prepare capture handle", snd_strerror(error) );
                }

            }else if( rc < 0 ){
                throw ErrorException("Can`t write to PCM device", snd_strerror( rc ) );
            }

            loop_count += 1;
    }
}

void AudioPlayback::openDevice(const char *device) throw (std::exception)
{
    if( m_isDeviceOpen )
          throw ErrorException("Playback Device is already open:", device);

    if( device == nullptr )
          throw ErrorException("NullPointerException by device:", device );

    int error = 0;
    int dir;
    unsigned int rate = m_bitrate;
    unsigned int channels = 1;
    snd_pcm_uframes_t frames = 1024;
    AUDIO_PCM_FORMAT format = SND_PCM_FORMAT_S16_LE;

    if( (error = snd_pcm_open( &playback_handle , device , SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        throw ErrorException("cannot open audio device:", device);
    }

    Logger::instance().info("Audio interface is open ... ");

    if( (error = snd_pcm_hw_params_malloc( &hw_params)) < 0 )
    {
        throw ErrorException("cannot allocate hardware parameter structure:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params allocated ...");

    if( (error = snd_pcm_hw_params_any ( playback_handle , hw_params)) < 0)
    {
        throw ErrorException("cannot initialize hardware aprameter structure:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params initialized");

    if ((error = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        throw ErrorException("cannot set access type:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params access setted");

    if( (error = snd_pcm_hw_params_set_format (playback_handle, hw_params, format)) < 0)
    {
        throw ErrorException("cannot set sample format:", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - format setted");


    if( (error = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &rate, 0)) < 0)
    {
        throw ErrorException("cannot set sample rate", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - rate setted ");

    if ((error = snd_pcm_hw_params_set_channels (playback_handle, hw_params, channels )) < 0)
    {
        throw ErrorException("cannot set channel count", snd_strerror(error) );
    }

    snd_pcm_hw_params_set_period_size_near( playback_handle, hw_params, &frames, &dir);

    Logger::instance().info("Audio HW Params - channels setted");

    if ((error = snd_pcm_hw_params (playback_handle, hw_params)) < 0)
    {
        throw ErrorException("cannot set parameters", snd_strerror(error) );
    }

    Logger::instance().info("Audio HW Params - setted");
    m_isDeviceOpen = true;
}

void AudioPlayback::closeDevice(void)
{
    snd_pcm_drain(playback_handle);
    snd_pcm_close(playback_handle);

    m_isDeviceOpen = false;
}
