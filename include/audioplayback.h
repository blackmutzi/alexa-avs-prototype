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
#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include "logger.h"
#include "audio.h"

namespace Audio
{

    class AudioPlayback
    {
        public:
            AudioPlayback();
            AudioPlayback( unsigned int bitrate );
            ~AudioPlayback(){}

            void setBitrate( unsigned int bitrate );
            void openDevice( const char * device ) throw ( std::exception );
            void setMutedStatus( bool status );
            void playback( char * audio_bytes , snd_pcm_uframes_t frames , int bits_per_sample ) throw (std::exception);
            void closeDevice(void);

        private:

            unsigned int m_bitrate;


            AUDIO_PCM_HANDLE    * playback_handle;
            AUDIO_PCM_HW_PARAMS * hw_params;
            AUDIO_PCM_BUFFER    * buffer;

            bool m_isDeviceOpen;
            bool m_muted;
    };
}

#endif // AUDIOPLAYBACK_H
