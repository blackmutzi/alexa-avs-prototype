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
#include "include/httpcontent.h"

using namespace Network::HTTP;

HTTPContent::HTTPContent( std::string content , HTTPContentType type  )
    : m_json_content( content ) ,
      m_binary_content( (char*) m_json_content.c_str() ) ,
      m_binary_type( type )
{}

HTTPContent::HTTPContent( char * content , HTTPContentType type  )
    : m_json_content("") ,
      m_binary_content( content ) ,
      m_binary_type( type )
{}

HTTPContent::~HTTPContent(){}

void HTTPContent::setContentType(HTTPContentType type)
{
    m_binary_type = type;
}

void HTTPContent::setContent( char * content )
{
   m_binary_content = content;
}

char * HTTPContent::getContent(void) const
{
    return m_binary_content;
}

HTTPContentType HTTPContent::getContentType(void) const
{
    return m_binary_type;
}



