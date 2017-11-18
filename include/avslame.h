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
#ifndef AVSLAME_H
#define AVSLAME_H

#include "stdafx.h"

class AVSLame
{
    public:
        AVSLame();
        ~AVSLame();

        int set_in_samplerate( int rate );
        int set_VBR( vbr_mode mode );
        int set_decode_only( int );
        int init_params();

        int decode1_headers_mp3data(  unsigned char * mp3_buf , size_t mp3_len , short pcm_l[] , short pcm_r[] , mp3data_struct * mp3_data );
        int decode1_headers( unsigned char * mp3_buf , size_t mp3_len , short pcm_l[] , short pcm_r[] );
        int decode1( unsigned char *mp3_buf, size_t mp3_len, short pcm_l[] , short pcm_r[] );

    private:

        lame_t m_lame_handler;
        hip_t  m_hip_handler;
        mp3data_struct m_mp3_data;

};

#endif // AVSLAME_H
