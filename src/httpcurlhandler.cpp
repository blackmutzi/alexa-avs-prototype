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
#include "include/httpcurlhandler.h"

using namespace Network::HTTP;

HTTPCurlHandler::HTTPCurlHandler()
    : m_curl_handle(nullptr)
{}

void HTTPCurlHandler::init()
{
    curl_global_init(CURL_GLOBAL_ALL);

    if( m_curl_handle == nullptr )
            m_curl_handle = curl_easy_init();

}

void HTTPCurlHandler::setResponseCallback( HTTPResponseMemory * chunk ) throw ( std::exception )
{
   if( m_curl_handle == nullptr )
            throw ErrorException("NullpointerException: m_curl_handle ");

   m_response_content = chunk;

   //curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, writeHeaderFunction);

   /* send all data to this function  */
   curl_easy_setopt( m_curl_handle, CURLOPT_WRITEFUNCTION, &HTTPCurlHandler::WriteMemoryCallback );

   /* we pass our 'chunk' struct to the callback function */
   curl_easy_setopt( m_curl_handle , CURLOPT_WRITEDATA, m_response_content );
}


void HTTPCurlHandler::doIt(void) throw ( std::exception )
{
   CURLcode res;

   if( m_curl_handle == nullptr )
            throw ErrorException("NullpointerException: m_curl_handle");

   /* get it! */
   res = curl_easy_perform(m_curl_handle);

   /* check for errors */
   if(res != CURLE_OK)
       throw ErrorException("curl_easy_perfom failed:" , curl_easy_strerror(res) );

   if( res == CURLE_OK )
   {
       Logger::instance().info("HTTPCurlHandler: CURL Ok: check");
       Logger::instance().info("HTTPCurlHandler: bytes retreived", (long) m_response_content->size );
   }
}

size_t HTTPCurlHandler::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct HTTPResponseMemory *mem = (struct HTTPResponseMemory *)userp;

    mem->memory = (char*) realloc(mem->memory, sizeof(char) * ( mem->size + realsize + 1) );
    if(mem->memory == NULL)
    {
      /* out of memory! */
      Logger::instance().warn("not enough memory - realloc returned null ");
      return 0;
    }

    memset( &(mem->memory[mem->size]), 0 , mem->size + realsize + 1);
    memcpy(&(mem->memory[mem->size]), contents, realsize );
    mem->size += realsize + 1;

    return realsize;
}

void HTTPCurlHandler::clean(void)
{
    curl_easy_cleanup(m_curl_handle);

    curl_global_cleanup();
}


CURL * HTTPCurlHandler::getHandle(void)
{
    return m_curl_handle;
}

