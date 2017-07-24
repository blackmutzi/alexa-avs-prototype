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
#include "include/http2header.h"

using namespace Network::HTTP;

std::string HTTP2Header::HTTP2_HEADERMSG_METHOD = ":method =";
std::string HTTP2Header::HTTP2_HEADERMSG_SHEME  = ":scheme =";
std::string HTTP2Header::HTTP2_HEADERMSG_PATH   = ":path =";
std::string HTTP2Header::HTTP2_HEADERMSG_AUTHORIZATION = "authorization = Bearer ";
std::string HTTP2Header::HTTP2_HEADERMSG_CONTENT_TYPE  = "content-type =";
std::string HTTP2Header::HTTP2_HEADERMSG_BOUNDARY = "boundary=";

std::string HTTP2Header::HTTP2_PING  =  "/ping";
std::string HTTP2Header::HTTP2_EVENTS_ENDPOINT    =  "/v20160207/events";
std::string HTTP2Header::HTTP2_DIRECTIVE_ENDPOINT =  "/v20160207/directives";

HTTP2Header::HTTP2Header()
    : m_methode("") ,
      m_sheme("") ,
      m_path("") ,
      m_access_token("") ,
      m_content_type("") ,
      m_boundary("")
{}

HTTP2Header::HTTP2Header(const string access_token, const string event_path)
    : m_methode("") ,
      m_sheme("") ,
      m_path( event_path ) ,
      m_access_token( access_token ) ,
      m_content_type("") ,
      m_boundary("")
{}

HTTP2Header::~HTTP2Header()
{
   //Nothing to do ...
}

void HTTP2Header::setMethode(const string methode)
{
    m_methode = methode;
}

void HTTP2Header::setSheme(const string scheme)
{
    m_sheme = scheme;
}

void HTTP2Header::setPath(const string path)
{
    m_path = path;
}

void HTTP2Header::setAuthorization(const string access_token)
{
    m_access_token = access_token;
}

void HTTP2Header::setContentType(const string content_type)
{
    m_content_type = content_type;
}

void HTTP2Header::setBoundary(const string boundary)
{
    m_boundary = boundary;
}

const std::string & HTTP2Header::getMethode(void) const
{
    return m_methode;
}

const std::string & HTTP2Header::getSheme(void) const
{
    return m_sheme;
}

const std::string & HTTP2Header::getPath(void) const
{
    return m_path;
}

const std::string & HTTP2Header::getAccessToken(void) const
{
    return m_access_token;
}

const std::string & HTTP2Header::getContentType(void) const
{
    return m_content_type;
}

const std::string & HTTP2Header::getBoundary(void) const
{
    return m_boundary;
}
