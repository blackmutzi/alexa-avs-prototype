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
#include "include/httpclientdirectiveevent.h"

using namespace Network::HTTP;

HTTPClientDirectiveEvent::HTTPClientDirectiveEvent()
    : m_audio_object_count(0) ,
      m_json_object_count(0)
{}

HTTPClientDirectiveEvent::~HTTPClientDirectiveEvent()
{
    JsonObject * json_object;
    AudioObject * audio_object;

    for( unsigned int json_count = 0; json_count < m_json_object_count; json_count++ )
    {
       json_object = getJsonObject( json_count );
       json_object->size = 0;
       delete[] json_object->json_string;
       delete json_object;
    }

    for( unsigned int audio_count = 0; audio_count < m_audio_object_count; audio_count++ )
    {
        audio_object = getAudioObject( audio_count );
        audio_object->size = 0;
        delete[] audio_object->memory;
        delete audio_object;
    }
}

void HTTPClientDirectiveEvent::addJsonObject(char *data, size_t size)
{
    JsonObject * object = new JsonObject();
    object->size = size;
    object->json_string = new char[ size + 1];

    memset( object->json_string , 0 , size + 1 );
    memcpy( object->json_string , data , size );

    m_json_object_list.push_back( object );
    m_json_object_count++;
}

void HTTPClientDirectiveEvent::addAudioObject(char *data, size_t size)
{
    AudioObject * object = new AudioObject();
    object->size   = size;
    object->memory = new char[ size ];

    memset( object->memory , 0 , size );
    memcpy( object->memory , data , size );

    m_audio_object_list.push_back( object );
    m_audio_object_count++;
}

JsonObject * HTTPClientDirectiveEvent::getJsonObject(unsigned int position) const
{
    return m_json_object_list[ position ];
}

AudioObject * HTTPClientDirectiveEvent::getAudioObject(unsigned int position) const
{
    return m_audio_object_list[ position ];
}

unsigned int HTTPClientDirectiveEvent::getJsonObjectCount(void)
{
    return m_json_object_count;
}

unsigned int HTTPClientDirectiveEvent::getAudioObjectCount(void)
{
    return m_audio_object_count;
}
