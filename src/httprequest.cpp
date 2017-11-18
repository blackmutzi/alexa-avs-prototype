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
#include "include/httprequest.h"

using namespace Network::HTTP;

HTTPRequest::HTTPRequest( const std::string & url , HTTPResponse * response )
    :  HTTPCurlHandler() ,
       m_response( response ) ,
       m_response_memory( new HTTPResponseMemory() ) ,
       m_base_url( url ) ,
       m_header_content( NULL )
{
    HTTPCurlHandler::init();

    curl_easy_setopt( m_curl_handle , CURLOPT_URL, m_base_url.c_str() );
    curl_easy_setopt( m_curl_handle , CURLOPT_VERBOSE, 1L);
    curl_easy_setopt( m_curl_handle , CURLOPT_SSL_VERIFYPEER , false );
    curl_easy_setopt( m_curl_handle , CURLOPT_SSL_VERIFYHOST , 2 );

    m_response_memory->memory = (char*) malloc(1);
    m_response_memory->size   = 0;
    HTTPCurlHandler::setResponseCallback( m_response_memory );
}

HTTPRequest::~HTTPRequest()
{
    free( m_response_memory->memory );
    m_response_memory->size = 0;
    delete m_response_memory;
}

void HTTPRequest::setPostData( string data )
{
    const char * origin_post_data = data.c_str();

    curl_easy_setopt(m_curl_handle, CURLOPT_POST, 1);
    curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDSIZE, (long) strlen( origin_post_data ) );
    curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDS, origin_post_data );
}

void HTTPRequest::addHeaderData(string header_data)
{
     m_header_content = curl_slist_append( m_header_content , header_data.c_str() );
}

void HTTPRequest::writeHeaderData(void)
{
    curl_easy_setopt( m_curl_handle  , CURLOPT_HTTPHEADER    ,  m_header_content );
}

void HTTPRequest::doIt(void) throw (std::exception )
{
    // send request
    HTTPCurlHandler::doIt();

    // make a copy of response data
    char * copy_data = (char*) malloc( m_response_memory->size );
    memcpy( copy_data , m_response_memory->memory ,m_response_memory->size );

    // save response data
    m_response->setContent( copy_data );
}

void HTTPRequest::clean()
{
    HTTPCurlHandler::clean();
}




