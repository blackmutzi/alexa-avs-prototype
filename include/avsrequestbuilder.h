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
#ifndef AVSREQUESTBUILDER_H
#define AVSREQUESTBUILDER_H

#include "stdafx.h"
#include "logger.h"
#include "http2client.h"
#include "httpresponse.h"
#include "http2header.h"
#include "httpheader.h"
#include "httpcontent.h"
#include "httpheaderfactory.h"
#include "httpcontentfactory.h"
#include "jsonfactory.h"

using namespace Network::HTTP;
using namespace Network::HTTP2;

struct AVSHeaderData
{
    nghttp2_nv * header;
    size_t header_len;
};

struct AVSSourceData
{
    char * source;
    size_t source_len;
};

struct AVSStreamData
{
    AVSHeaderData * header_data;
    AVSSourceData * source_data;
};

/*!
 * Builder & Invoker Class
 * \brief The AVSRequestBuilder class
 */
class AVSRequestBuilder
{
    public:
        AVSRequestBuilder( HTTP2Client * client , const std::string access_token , const std::string boundary );
        ~AVSRequestBuilder(){}

        void invokeDownchannelStream(void);

        StreamQueueData * buildPingStream(void);
        StreamQueueData * buildEventStream( const std::string json_object );
        StreamQueueData * buildRecognizeStream( const AVSJson::SynchronizeStateEvent * event , const std::string dialog_request_id , const char * audio_data , size_t audio_len );
        StreamQueueData * buildSyncStream(const AVSJson::SynchronizeStateEvent * event );

        void changeAccessToken( std::string access_token );
        void changeClient( HTTP2Client * client );

    protected:

        void buildMainHeader( const HTTP2Header * input , AVSHeaderData * ouput );
        void buildMultiplexHeader( const HTTP2Header * input , AVSHeaderData * output);
        void buildBody( const HTTPHeader * header_input , const HTTPContent * content_input , AVSSourceData * output );
        void buildMultiplexBody(  const HTTPRequestMultipartContent * content , size_t audio_len , AVSSourceData * output );

    private:

        HTTP2Client * m_http2_client;
        std::string   m_access_token;
        std::string   m_boundary;
};

#endif // AVSREQUESTBUILDER_H
