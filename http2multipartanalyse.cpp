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
#include "include/http2multipartanalyse.h"

using namespace Network::HTTP2;

HTTP2MultipartAnalyse::HTTP2MultipartAnalyse()
    : m_directive_bytes( nullptr ) ,
      m_audio_bytes( nullptr ) ,
      m_directive_allocate_count( 0 ) ,
      m_audio_allocate_count( 0 )
{}

void HTTP2MultipartAnalyse::analyse( char * content , size_t size )
{
    std::vector< size_t > application_json_positions;
    std::vector< size_t > application_octet_positions;

    size_t directive_begin_pos  = 0;
    size_t directive_end_pos    = 0;
    size_t directive_size       = 0;
    size_t boundary_begin_pos   = 0;
    size_t boundary_end_pos     = 0;
    size_t boundary_size        = 0;
    size_t audio_begin_pos      = 0;
    size_t audio_end_pos        = 0;
    size_t audio_size           = 0;
    size_t app_json_len         = strlen("Content-Type: application/json; charset=UTF-8");
    size_t app_octet_len        = strlen("Content-Type: application/octet-stream");
    size_t MAX_DIRECTIVE_COUNT  = 2;
    size_t MAX_AUDIO_COUNT      = 1;

    unsigned int REAL_DIRECTIVE_COUNT = 0;
    unsigned int REAL_AUDIO_COUNT     = 0;
    unsigned int REAL_BOUNDARY_COUNT  = 0;

    char * real_boundary = nullptr;
    m_directive_bytes    = nullptr;
    m_audio_bytes        = nullptr;
    m_directive_allocate_count = 0;
    m_audio_allocate_count     = 0;

    // Get Positions from Content-Type: application/json; charset=UTF-8 Positions ( MAXIMUM: 2 )
    application_json_positions  = getEntryPositions( content , size , MAX_DIRECTIVE_COUNT , "Content-Type: application/json; charset=UTF-8" , &REAL_DIRECTIVE_COUNT );

    // Get positions from Content-Type: application/octet-stream ( MAXIMUM: 1 )
    application_octet_positions = getEntryPositions( content , size , MAX_AUDIO_COUNT , "Content-Type: application/octet-stream" , &REAL_AUDIO_COUNT );

    // if this true then no DIRECTIVE found in content
    if( REAL_DIRECTIVE_COUNT == 0 )
                return;

    // allocate array DIRECTIVE_BYTES
    m_directive_bytes = new DIRECTIVE_BYTES[ REAL_DIRECTIVE_COUNT ];
    m_directive_allocate_count = REAL_DIRECTIVE_COUNT;

    for( unsigned int directive_loop = 0; directive_loop < REAL_DIRECTIVE_COUNT ; directive_loop++ )
    {
        // content_type_begin_pos + length_of_content_type_application_json + 4 bytes ( \n\n\n\n )
        directive_begin_pos = application_json_positions[ directive_loop ] + app_json_len  + 4;
        // search for next (\n) byte and get position
        directive_end_pos   = directive_begin_pos + nextEntryPoint( content , directive_begin_pos , '\n' );
        // now we have directive begin & end positions
        directive_size  = directive_end_pos - directive_begin_pos;
        // now we have directive_size

        // pick up custom boundary ( only one )
        if( directive_loop == 0 )
        {
            boundary_begin_pos = directive_end_pos + 1;
            boundary_end_pos   = boundary_begin_pos + nextEntryPoint( content , boundary_begin_pos , '\n' );
            boundary_size      = boundary_end_pos - boundary_begin_pos;
        }

        // allocate directive bytes
        (&m_directive_bytes[ directive_loop ])->bytes = new char [ directive_size ];
        // save directive_size
        (&m_directive_bytes[ directive_loop ])->size  = directive_size;
        // save directive data
        memset( (&m_directive_bytes[ directive_loop ])->bytes , 0 , directive_size );
        memcpy( (&m_directive_bytes[ directive_loop ])->bytes , content + directive_begin_pos , directive_size );
    }

    // if this true then no BINARY AUDIO BYTES found in content
    if( REAL_AUDIO_COUNT == 0 )
            return;

    // GET ALL Boundary Positions ( no needed )
    // boundary_positions = getEntryPositions( content , size , MAX_BOUNDARY_COUNT , real_boundary , &REAL_BOUNDARY_COUNT );

    // allocate array AUDIO_BYTES
    m_audio_bytes = new AUDIO_BYTES[ REAL_AUDIO_COUNT ];
    m_audio_allocate_count = REAL_AUDIO_COUNT;

    for( unsigned int audio_loop = 0; audio_loop < REAL_AUDIO_COUNT; audio_loop++ )
    {
        // content_type_application_octet_begin_pos + length_of_content_type_application_octet + 4 bytes ( \n\n\n\n )
        audio_begin_pos  = application_octet_positions[ audio_loop ] + app_octet_len + 4;
        // audio_end_pos = next_boundary_position - 1 byte (\n)
        audio_end_pos    = size - (boundary_size + 2) - 1;
        // now we have binary_audio_size
        audio_size = audio_end_pos - audio_begin_pos;

        // allocate audio bytes
        (&m_audio_bytes[audio_loop])->bytes = new char[ audio_size ];
        // save audio_size
        (&m_audio_bytes[audio_loop])->size = audio_size;
        // write / save audio data
        memset( (&m_audio_bytes[ audio_loop ])->bytes , 0 , audio_size );
        memcpy( (&m_audio_bytes[ audio_loop ])->bytes , content + audio_begin_pos , audio_size );
    }
}

void HTTP2MultipartAnalyse::update( HTTPClientDirectiveEvent * event )
{
    char * audio_bytes  = nullptr;
    char * json_bytes   = nullptr;
    size_t current_size = 0;

    for( unsigned int audio_count = 0; audio_count < m_audio_allocate_count; audio_count++ )
    {
        audio_bytes  = (&m_audio_bytes[ audio_count] )->bytes;
        current_size = (&m_audio_bytes[ audio_count] )->size;

        event->addAudioObject( audio_bytes , current_size );

        delete[] audio_bytes;
        audio_bytes = nullptr;
    }
    if( m_audio_allocate_count != 0 )
    {
           delete[] m_audio_bytes;
           m_audio_bytes = nullptr;
           m_audio_allocate_count = 0;
    }

    for( unsigned int directive_count = 0; directive_count < m_directive_allocate_count; directive_count++ )
    {
        json_bytes   = (&m_directive_bytes[ directive_count] )->bytes;
        current_size = (&m_directive_bytes[ directive_count] )->size;

        event->addJsonObject( json_bytes , current_size );

        delete[] json_bytes;
        json_bytes = nullptr;
    }

    if( m_directive_allocate_count != 0 )
    {
           delete[] m_directive_bytes;
           m_directive_bytes = nullptr;
           m_directive_allocate_count = 0;
    }
}

void HTTP2MultipartAnalyse::debug_write_file(const char *file, const char *content, size_t content_size)
{
    int fd = open( file , O_WRONLY );
    write( fd , content , content_size );
    close( fd );
}

size_t HTTP2MultipartAnalyse::nextEntryPoint( char * content , size_t offset , char delim )
{
    char * next = content + offset;
    size_t pos  = 0;

    for( next; next != NULL; next++ )
    {
        if( *next == delim )
               break;
        pos++;
    }
    return pos;
}

std::vector< size_t > HTTP2MultipartAnalyse::getEntryPositions( char * content , size_t size , unsigned int amount , std::string needle , unsigned int * real_amount )
{
    char * next = content;
    size_t pos  = 0;
    size_t real_number = 0;
    unsigned int while_counter = 0;
    std::vector< size_t > list_of_positions;

    while( while_counter < amount )
    {
        if( while_counter == 0 )
        {
           if( pos >= size )
                 break;

           pos = search( next , pos , size - pos , needle );

           if( pos == (unsigned long) -1 )
                  break;

           *real_amount += 1;
           list_of_positions.push_back( pos );
           while_counter++;
           continue;
        }

        // Offset
        pos = pos + strlen( needle.c_str() );

        if( pos >= size )
             break;

        // Next Search
        real_number = search( next , pos , size - pos , needle );
        pos += real_number;

        if( real_number == (unsigned long) -1 )
               break;

        *real_amount += 1;
        list_of_positions.push_back( pos );
        while_counter++;
    }

    return list_of_positions;
}

size_t HTTP2MultipartAnalyse::search( char * content , size_t offset , size_t size , std::string needle )
{
    std::string content_data;
    content_data.resize( size );
    size_t return_pos = 0;
    memcpy( (char*) content_data.data() , content + offset , size );
    return_pos = content_data.find( needle );
    return return_pos;
}
