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
#include "include/fileloader.h"

FileLoader::FileLoader()
    : m_file_name("")
{}

FileLoader::FileLoader(const string file_name)
    : m_file_name( file_name )
{}

void FileLoader::setFileName( const std::string file_name )
{
    m_file_name = file_name;
}

void FileLoader::load()
{
    ifstream file_stream;
    std::string line;

    file_stream.open( m_file_name.c_str() );

    while( std::getline( file_stream , line ) )
    {
        onLineEvent( line );
    }

    file_stream.close();

    onEndOfFileEvent();
}

void FileLoader::replaceAll( string & str , string search , string replace )
{
    if( search.empty() )
           return;

    size_t start_pos = 0;

    while((start_pos = str.find( search , start_pos)) != std::string::npos) {
           str.replace(start_pos, search.length(),  replace );
           start_pos += replace.length();
    }
}

std::string FileLoader::convertTo( long number )
{
    stringstream convert_stream;
    convert_stream << number;
    return convert_stream.str();
}

std::string FileLoader::convertTo( bool boolean )
{
    stringstream convert_stream;

    if( boolean )
    {
        convert_stream << "TRUE";

    } else{

        convert_stream << "FALSE";
    }

    return convert_stream.str();
}


