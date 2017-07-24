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
#include "include/bytefileloader.h"

ByteFileLoader::ByteFileLoader( const char * file_name )
    : m_file_name( file_name )
{}

void ByteFileLoader::init( struct FileBytes * store )
{
    store->bytes = nullptr;
    store->size  = 0;
}

void ByteFileLoader::readBytes( struct FileBytes * store )
{
    FILE * fp;
    size_t file_size = 0;
    char * buffer = nullptr;

    fp = fopen( m_file_name , "rb");
    fseek( fp , 0 , SEEK_END );
    file_size = ftell( fp );
    buffer = (char*) malloc( sizeof(char) * file_size );
    memset( buffer, 0 , file_size );

    fseek( fp , 0 , SEEK_SET );
    fread( buffer , 1 , file_size , fp );
    fclose( fp );

    store->bytes = buffer;
    store->size  = file_size;
}

void ByteFileLoader::clean( struct FileBytes * store )
{
    if( store->bytes != nullptr )
    {
        free( store->bytes );
    }
    store->size = 0;
}
