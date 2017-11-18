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
#include "include/avsplayback.h"

AVSPlayback::AVSPlayback()
    : m_playback_handle( new AudioPlayback() )
{}

AVSPlayback::~AVSPlayback()
{
    delete m_playback_handle;
}

void AVSPlayback::mute(void)
{
    // if true playback is muted
    m_playback_handle->setMutedStatus( true );
}

void AVSPlayback::unmute(void)
{
    m_playback_handle->setMutedStatus( false );
}

void AVSPlayback::mp3Playback(char *mp3_data, size_t mp3_size)
{
    int num_samples = 0;
    int current_samples = 0;
    int sample_rate = 0;
    int bitrate     = 0;
    int totalframes = 0;
    int framenum    = 0;
    int framesize   = 0;
    int channels    = 0;
    int samplecount = 0;
    int bits_per_sample = 0;
    size_t read_bytes = 128;
    size_t offset   = 0;

    AVSLame lame;
    mp3data_struct * mp3_struct_data = new mp3data_struct();
    size_t pcm_buffer_size = 0;
    bool header_parse = false;

    const int PCM_SIZE = 8192;
    char pcm_buffer_l[ PCM_SIZE * sizeof(short int) ];
    char pcm_buffer_r[ PCM_SIZE * sizeof(short int) ];
    char * pcm_buffer;

    lame.set_decode_only(1);
    lame.init_params();

    num_samples = lame.decode1_headers_mp3data(  (unsigned char * ) mp3_data , mp3_size , reinterpret_cast<short int*>( &pcm_buffer_l[0] ),reinterpret_cast<short int*>( &pcm_buffer_r[0] ) , mp3_struct_data );
    while( read_bytes < mp3_size  )
    {
            if( mp3_struct_data->header_parsed == 1 && !header_parse )
            {
                    sample_rate = mp3_struct_data->samplerate;
                    bitrate     = mp3_struct_data->bitrate * 1000 ;
                    totalframes = mp3_struct_data->totalframes;
                    framenum    = mp3_struct_data->framenum;
                    framesize   = mp3_struct_data->framesize;
                    channels    = mp3_struct_data->stereo;

                    Logger::instance().info("SampleRate:", sample_rate );
                    Logger::instance().info("bitrate:", bitrate );
                    Logger::instance().info("channels", channels );
                    Logger::instance().info("framesize", framesize );

                    bits_per_sample = bitrate / ( sample_rate * channels );
                    pcm_buffer_size = num_samples * bits_per_sample ;
                    pcm_buffer = (char*) malloc( sizeof( char )  * pcm_buffer_size );
                    memset( pcm_buffer , 0 , pcm_buffer_size );
                    header_parse = true;

                    Logger::instance().info("pcm_buffer_size", pcm_buffer_size );

           }

            current_samples = lame.decode1( (unsigned char * ) mp3_data , read_bytes , reinterpret_cast<short int*>( &pcm_buffer_l[0] ) , reinterpret_cast<short int*>( &pcm_buffer_r[0] ));
            num_samples += current_samples;

            if( current_samples == 0 )
                    break;

            memcpy( pcm_buffer + offset , &pcm_buffer_l[0] , current_samples * bits_per_sample );
            offset += current_samples * bits_per_sample ;
            pcm_buffer = (char*) realloc( pcm_buffer , sizeof( char) * ( offset * 2 ) );
            read_bytes += 128;
    }

    delete mp3_struct_data;

    try{

        m_playback_handle->setMutedStatus( false );

        m_playback_handle->setBitrate( sample_rate );

        m_playback_handle->openDevice("default");

        m_playback_handle->playback( pcm_buffer , num_samples , bits_per_sample );

        m_playback_handle->closeDevice();

        free( pcm_buffer );

    }catch( ErrorException e )
    {
        Logger::instance().error( e.what() );
    }
}

