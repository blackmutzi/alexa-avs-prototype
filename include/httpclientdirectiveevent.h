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
#ifndef HTTPCLIENTDIRECTIVEEVENT_H
#define HTTPCLIENTDIRECTIVEEVENT_H

#include "network.h"

namespace Network
{
    namespace HTTP
    {
         struct JsonObject
         {
             char * json_string;
             size_t size;
         };

         struct AudioObject
         {
             AudioObject()
             {
                memory = nullptr;
                size = 0;
             }
             ~AudioObject(){}

             char * memory;
             size_t size;
         };

         using JsonObjectList  = std::vector< JsonObject *>;
         using AudioObjectList = std::vector< AudioObject *>;

         //@Event Class
         class HTTPClientDirectiveEvent
         {
            public:
                 HTTPClientDirectiveEvent();
                 ~HTTPClientDirectiveEvent();

                 void addJsonObject( char * data , size_t size );
                 void addAudioObject( char * data , size_t size );

                 JsonObject  * getJsonObject( unsigned int position ) const ;
                 AudioObject * getAudioObject( unsigned int position ) const ;

                 unsigned int getJsonObjectCount(void);
                 unsigned int getAudioObjectCount(void);

            private:

                JsonObjectList m_json_object_list;
                AudioObjectList m_audio_object_list;

                unsigned int m_audio_object_count;
                unsigned int m_json_object_count;

         };

    }
}

#endif // HTTPCLIENTDIRECTIVEEVENT_H
