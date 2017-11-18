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
#ifndef AVSPLAYBACK_H
#define AVSPLAYBACK_H

#include "stdafx.h"
#include "audioplayback.h"
#include "avslame.h"

using namespace Audio;

class AVSPlayback
{
    public:
        AVSPlayback();
        ~AVSPlayback();

        /*!
         * cancel Audio playback
         * Thread Safe
         * \brief cancel
         */
        void mute(void);
        void unmute(void);

        /*!
         * Use libmp3lame
         * play mp3 audio
         *
         * \brief mp3Playback
         * \param mp3_data
         * \param mp3data_size
         */
        void mp3Playback( char * mp3_data , size_t mp3data_size );

    private:

        AudioPlayback * m_playback_handle;
};

#endif // AVSPLAYBACK_H
