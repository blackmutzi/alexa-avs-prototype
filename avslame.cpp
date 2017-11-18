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
#include "include/avslame.h"

AVSLame::AVSLame()
{
    m_lame_handler = lame_init();
    m_hip_handler  = hip_decode_init();
}

AVSLame::~AVSLame()
{
    lame_close( m_lame_handler );
    hip_decode_exit( m_hip_handler );
}

int AVSLame::set_in_samplerate( int rate )
{
    return lame_set_in_samplerate( m_lame_handler , rate );
}

int AVSLame::set_VBR( vbr_mode mode )
{
    return lame_set_VBR( m_lame_handler , mode );
}

int AVSLame::set_decode_only(int only )
{
    return lame_set_decode_only( m_lame_handler , only );
}

int AVSLame::init_params(void)
{
    return lame_init_params( m_lame_handler );
}

int AVSLame::decode1(unsigned char *mp3_buf, size_t mp3_len, short pcm_l[] , short pcm_r[] )
{
    return hip_decode1( m_hip_handler , mp3_buf , mp3_len , pcm_l , pcm_r );
}

int AVSLame::decode1_headers( unsigned char * mp3_buf , size_t mp3_len , short pcm_l[] , short pcm_r[] )
{
    return hip_decode1_headers( m_hip_handler , mp3_buf , mp3_len , pcm_l , pcm_r , &m_mp3_data );
}

int AVSLame::decode1_headers_mp3data(  unsigned char * mp3_buf , size_t mp3_len , short pcm_l[] , short pcm_r[] , mp3data_struct * mp3_data )
{
    return hip_decode1_headers( m_hip_handler , mp3_buf , mp3_len , pcm_l , pcm_r , mp3_data );
}
