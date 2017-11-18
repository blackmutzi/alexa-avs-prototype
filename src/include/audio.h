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
#ifndef AUDIO_H
#define AUDIO_H

#include "stdafx.h"

namespace Audio
{
    using AUDIO_PCM_HANDLE         = snd_pcm_t;
    using AUDIO_PCM_FORMAT         = snd_pcm_format_t;
    using AUDIO_PCM_HW_PARAMS      = snd_pcm_hw_params_t;
    using AUDIO_PCM_FRAMES         = snd_pcm_uframes_t;
    using AUDIO_PCM_BUFFER         = char;

    //Output
    class AudioPlayerFactory;
    class AudioPlayer;
    class AudioPlayerActivity;
    class AudioPlayback;

    //Input
    class AudioCapture;
    class AudioInputFormat;

    namespace Alert
    {
        class Alert;
        class AlertEventListener;
        class AlertHandler;
        class AlertManager;
        class AlertManagerFactory;
        class AlertScheduler;
        class AlertDataStore;
        class AlertFileDataStore;
    }
}

#endif // AUDIO_H
