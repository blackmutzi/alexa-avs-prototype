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
#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include "logger.h"
#include "audio.h"

namespace Audio
{
   class AudioCapture
   {
       public:
          AudioCapture();
          ~AudioCapture();

          /*!
           * \brief openAudioDevice
           * \param device: hw:0 or default
           *
           *  rate=44100
           *  channels=1
           *  16bit samples
           */
          AUDIO_PCM_HANDLE * openDevice( const char * device ) throw( std::exception );

          /*!
           * \brief captureLoop
           * \return
           */
          const char * captureLoop( snd_pcm_uframes_t & frames );

          /*!
           * \brief closeAudioDevice
           */
          void closeDevice(void);

        protected:

          static void timer( int * flag );

        private:

            AUDIO_PCM_HANDLE    * capture_handle;
            AUDIO_PCM_HW_PARAMS * hw_params;
            AUDIO_PCM_BUFFER    * buffer;

            bool m_isDeviceOpen;
            bool m_isCaptureStarted;

   };

}

#endif // AUDIOCAPTURE_H
