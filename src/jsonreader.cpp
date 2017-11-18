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
#include "include/jsonreaderdirective.h"

using namespace AVSJson;

JsonReaderDirective::JsonReaderDirective()
    : m_rawjson_content("")
{}

JsonReaderDirective::JsonReaderDirective( const char * content )
    : m_rawjson_content("")
{
    m_json = json::parse( content );
}

JsonReaderDirective::JsonReaderDirective( const std::string json_content )
    : m_rawjson_content( json_content )
{
    m_json = json::parse( m_rawjson_content.c_str() );
}

void JsonReaderDirective::setJsonContent( const std::string json_content )
{
    m_rawjson_content = json_content;
    m_json = json::parse( m_rawjson_content.c_str() );
}

const std::string JsonReaderDirective::getHeaderNameSpace(void) const throw( std::exception )
{
    return m_json["directive"]["header"]["namespace"];
}

const std::string JsonReaderDirective::getHeaderName(void) const throw( std::exception )
{
    return m_json["directive"]["header"]["name"];
}

const std::string JsonReaderDirective::getHeaderMessageId(void) const throw( std::exception )
{
    return m_json["directive"]["header"]["messageId"];
}

const std::string JsonReaderDirective::getHeaderDialogRequestId(void) const throw( std::exception )
{
    return m_json["directive"]["header"]["dialogRequestId"];
}

const std::string JsonReaderDirective::getPayloadUrl(void) const throw( std::exception )
{
    return m_json["directive"]["payload"]["url"];
}

const std::string JsonReaderDirective::getPayloadFormat(void) const throw( std::exception )
{
    return m_json["directive"]["payload"]["format"];
}

const std::string JsonReaderDirective::getPayloadToken(void) const throw( std::exception )
{
    return m_json["directive"]["payload"]["token"];
}

const std::string JsonReaderDirective::getPayloadMute(void) const throw( std::exception )
{
    return m_json["directive"]["payload"]["mute"];
}








