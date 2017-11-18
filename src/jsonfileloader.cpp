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
#include "include/jsonfileloader.h"

using namespace AVSJson;

JsonFileLoader::JsonFileLoader( const std::string json_file_name )
    : FileLoader( json_file_name ) ,
      m_json_object("")
{}

void JsonFileLoader::setHashmap( JsonHashMap map )
{
    m_json_hashmap = map;
}

void JsonFileLoader::addReplaceData( std::string search , std::string replace )
{
    m_json_hashmap.insert( std::pair<string,string>( search , replace ) );
}

const std::string JsonFileLoader::getJsonObject(void)
{
    return m_json_object;
}

void JsonFileLoader::onLineEvent( const string line )
{
    std::string search , replace , newline ;
    newline = line;

    for ( JsonHashMap::iterator it= m_json_hashmap.begin(); it!= m_json_hashmap.end(); ++it)
    {
        search  = (std::string) it->first;
        replace = (std::string) it->second;

        FileLoader::replaceAll( newline , search , replace );
    }

    m_parse_stream << newline << std::endl;
}

void JsonFileLoader::onEndOfFileEvent()
{
    m_json_object = m_parse_stream.str();
}


