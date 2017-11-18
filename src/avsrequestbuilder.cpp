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
#include "include/avsrequestbuilder.h"

AVSRequestBuilder::AVSRequestBuilder( HTTP2Client * client , const std::string access_token , const std::string boundary )
    : m_http2_client( client ) ,
      m_access_token( access_token ) ,
      m_boundary( boundary )
{}

void AVSRequestBuilder::changeClient( HTTP2Client * client )
{
    m_http2_client = client;
}

void AVSRequestBuilder::changeAccessToken(std::string access_token)
{
    m_access_token = access_token;
}

void AVSRequestBuilder::invokeDownchannelStream(void)
{
    HTTPHeaderFactory factory;
    HTTP2Header http2_header;
    AVSHeaderData header_data_output;

    http2_header = factory.create_HTTP2_GET( m_access_token , HTTP2Header::HTTP2_DIRECTIVE_ENDPOINT , "");
    buildMainHeader( &http2_header , &header_data_output );

    Logger::instance().info("--- Open Downchannel Stream ---");

    m_http2_client->getEventManager()->setBoundary( m_boundary );

    m_http2_client->getEventManager()->setDownChannelStatus( true );

    m_http2_client->initConnection( header_data_output.header , header_data_output.header_len , m_http2_client->getEventManager()->getStreamStatus() );

    m_http2_client->getEventManager()->setDownChannelStatus( false );

    Logger::instance().info("--- Close Downchannel Stream ---");
}

StreamQueueData * AVSRequestBuilder::buildPingStream(void)
{
    HTTPHeaderFactory header_factory;
    AVSHeaderData header_data_output;

    // Build HTTP/2 PING Header
    HTTP2Header http2_header = header_factory.create_HTTP2_GET( m_access_token , HTTP2Header::HTTP2_PING , "" );
    buildMainHeader( &http2_header , &header_data_output );

    Logger::instance().info("--- PUSH Ping Stream ---");
    StreamQueueData * data = new StreamQueueData();
    data->header_data = header_data_output.header;
    data->header_len  = header_data_output.header_len;
    data->source_data = nullptr;
    data->source_len  = 0;

    return data;
}

StreamQueueData * AVSRequestBuilder::buildRecognizeStream( const AVSJson::SynchronizeStateEvent * event , const std::string dialog_request_id , const char * audio_data , size_t audio_len )
{
    HTTPHeaderFactory    header_factory;
    HTTPContentFactory   content_factory;
    AVSJson::JsonFactory json_factory;
    HTTPRequestMultipartContent multipart_content;
    AVSHeaderData header_data_output;
    AVSSourceData source_data_output;

    // Build HTTP/2 Sync Header
    HTTP2Header http2_header   = header_factory.create_HTTP2_POST( m_access_token , HTTP2Header::HTTP2_EVENTS_ENDPOINT , m_boundary );

    // Build JSON Header
    HTTPHeader  json_header    = header_factory.createJSONHeader( m_boundary );

    // Build JSON Content
    HTTPContent json_content   = content_factory.createJSONContent( json_factory.createSpeechRecognizerEvent( event , dialog_request_id ) );

    // Build Audio Header
    HTTPHeader  audio_header   = header_factory.createAudioHeader( m_boundary );

    // Build Audio Content
    HTTPContent audio_content = content_factory.createAudioContent( audio_data );

    multipart_content.header_one  = &json_header;
    multipart_content.content_one = &json_content;
    multipart_content.header_two  = &audio_header;
    multipart_content.content_two = &audio_content;

    buildMultiplexHeader( &http2_header       ,  &header_data_output );
    buildMultiplexBody  ( &multipart_content , audio_len , &source_data_output );

    Logger::instance().info("--- BUIlD Recognize Stream ---");
    StreamQueueData * data = new StreamQueueData();
    data->header_data = header_data_output.header;
    data->header_len  = header_data_output.header_len;
    data->source_data = source_data_output.source;
    data->source_len  = source_data_output.source_len;

    return data;
}

StreamQueueData * AVSRequestBuilder::buildSyncStream(const SynchronizeStateEvent *event)
{
    HTTPHeaderFactory header_factory;
    HTTPContentFactory content_factory;
    AVSJson::JsonFactory json_factory;
    AVSHeaderData header_data_output;
    AVSSourceData source_data_output;

    // Build HTTP/2 Sync Header
    HTTP2Header http2_header = header_factory.create_HTTP2_POST( m_access_token , HTTP2Header::HTTP2_EVENTS_ENDPOINT , m_boundary );

    // Build JSON Header
    HTTPHeader  json_header  = header_factory.createJSONHeader( m_boundary );

    // Build JSON Content
    HTTPContent json_content = content_factory.createJSONContent( json_factory.createSystemSynchronizeEvent( event ) );

    buildMultiplexHeader( &http2_header , &header_data_output );
    buildBody( &json_header , &json_content , &source_data_output );

    Logger::instance().info("--- BUILD Sync Stream  ---");
    StreamQueueData * data = new StreamQueueData();
    data->header_data = header_data_output.header;
    data->header_len  = header_data_output.header_len;
    data->source_data = source_data_output.source;
    data->source_len  = source_data_output.source_len;

    return data;
}

StreamQueueData * AVSRequestBuilder::buildEventStream( const std::string json_object )
{
    HTTPHeaderFactory  header_factory;
    HTTPContentFactory content_factory;
    AVSHeaderData header_data_output;
    AVSSourceData source_data_output;

    HTTP2Header http2_header = header_factory.create_HTTP2_POST( m_access_token , HTTP2Header::HTTP2_EVENTS_ENDPOINT , m_boundary );
    HTTPHeader  json_header  = header_factory.createJSONHeader( m_boundary );
    HTTPContent json_content = content_factory.createJSONContent( json_object );

    buildMultiplexHeader( &http2_header , &header_data_output );
    buildBody( &json_header , &json_content , &source_data_output );

    Logger::instance().info("--- BUILD Event Stream  ---");
    StreamQueueData * data = new StreamQueueData();
    data->header_data = header_data_output.header;
    data->header_len  = header_data_output.header_len;
    data->source_data = source_data_output.source;
    data->source_len  = source_data_output.source_len;

    return data;
}

void AVSRequestBuilder::buildMainHeader( const HTTP2Header * header_data , AVSHeaderData * output )
{

//    nghttp2_nv hdrs[] = {

//        MAKE_NV2( ":method"       , header_data->getMethode().c_str() , strlen( header_data->getMethode().c_str() )) ,
//        MAKE_NV2( ":scheme"       , header_data->getSheme().c_str() , strlen(  header_data->getSheme().c_str() ) ) ,
//        MAKE_NV2( ":path"         , header_data->getPath().c_str()  , strlen( header_data->getPath().c_str() ))    ,
//        MAKE_NV2( "authorization" , header_data->getAccessToken().c_str() , strlen( header_data->getAccessToken().c_str() ))
//    };

    size_t methode_len = strlen( header_data->getMethode().c_str() );
    char * methode = new char[ methode_len + 1];
    memset( methode , 0 , methode_len + 1 );
    memcpy( methode , header_data->getMethode().c_str() , methode_len );

    size_t scheme_len = strlen( header_data->getSheme().c_str() );
    char * scheme = new char[ scheme_len + 1 ];
    memset( scheme , 0 , scheme_len + 1 );
    memcpy( scheme , header_data->getSheme().c_str() , scheme_len );

    size_t path_len = strlen( header_data->getPath().c_str() );
    char * path = new char[ path_len + 1 ];
    memset( path , 0 , path_len + 1 );
    memcpy( path , header_data->getPath().c_str() , path_len );

    size_t auth_len = strlen( header_data->getAccessToken().c_str() );
    char * auth = new char[ auth_len + 1 ];
    memset( auth , 0 , auth_len + 1);
    memcpy( auth , header_data->getAccessToken().c_str(), auth_len );

    nghttp2_nv * header = new nghttp2_nv[ 4 ];
    uint8_t flag = NGHTTP2_NV_FLAG_NONE;

    header[0].flags    = flag;
    header[0].name     = (uint8_t*) ":method";
    header[0].namelen  = 7;
    header[0].value    = (uint8_t*) methode;
    header[0].valuelen = methode_len;

    header[1].flags    = flag;
    header[1].name     = (uint8_t*) ":scheme";
    header[1].namelen  = 7;
    header[1].value    = (uint8_t*) scheme;
    header[1].valuelen = scheme_len;

    header[2].flags    = flag;
    header[2].name     = (uint8_t*) ":path";
    header[2].namelen  = 5;
    header[2].value    = (uint8_t*) path;
    header[2].valuelen = path_len;

    header[3].flags    = flag;
    header[3].name     = (uint8_t*) "authorization";
    header[3].namelen  = 13;
    header[3].value    = (uint8_t*) auth;
    header[3].valuelen = auth_len;

    output->header = header;
    output->header_len = 4;
}

void AVSRequestBuilder::buildMultiplexHeader( const HTTP2Header * header_data , AVSHeaderData * output)
{
    size_t methode_len = strlen( header_data->getMethode().c_str() );
    char * methode = new char[ methode_len + 1];
    memset( methode , 0 , methode_len + 1 );
    memcpy( methode , header_data->getMethode().c_str() , methode_len );

    size_t scheme_len = strlen( header_data->getSheme().c_str() );
    char * scheme = new char[ scheme_len + 1 ];
    memset( scheme , 0 , scheme_len + 1 );
    memcpy( scheme , header_data->getSheme().c_str() , scheme_len );

    size_t path_len = strlen( header_data->getPath().c_str() );
    char * path = new char[ path_len + 1 ];
    memset( path , 0 , path_len + 1 );
    memcpy( path , header_data->getPath().c_str() , path_len );

    size_t auth_len = strlen( header_data->getAccessToken().c_str() );
    char * auth = new char[ auth_len + 1 ];
    memset( auth , 0 , auth_len + 1);
    memcpy( auth , header_data->getAccessToken().c_str(), auth_len );

    size_t ct_len = strlen( header_data->getContentType().c_str() );
    char * ct = new char[ ct_len + 1 ];
    memset( ct , 0 , ct_len + 1 );
    memcpy( ct , header_data->getContentType().c_str() , ct_len );


    nghttp2_nv * header = new nghttp2_nv[ 5 ];
    uint8_t flag = NGHTTP2_NV_FLAG_NONE;

    header[0].flags    = flag;
    header[0].name     = (uint8_t*) ":method";
    header[0].namelen  = 7;
    header[0].value    = (uint8_t*) methode;
    header[0].valuelen = methode_len;

    header[1].flags    = flag;
    header[1].name     = (uint8_t*) ":scheme";
    header[1].namelen  = 7;
    header[1].value    = (uint8_t*) scheme;
    header[1].valuelen = scheme_len;

    header[2].flags    = flag;
    header[2].name     = (uint8_t*) ":path";
    header[2].namelen  = 5;
    header[2].value    = (uint8_t*) path;
    header[2].valuelen = path_len;

    header[3].flags    = flag;
    header[3].name     = (uint8_t*) "authorization";
    header[3].namelen  = 13;
    header[3].value    = (uint8_t*) auth;
    header[3].valuelen = auth_len;

    header[4].flags    = flag;
    header[4].name     = (uint8_t*) "content-type";
    header[4].namelen  = 12;
    header[4].value    = (uint8_t*) ct;
    header[4].valuelen = ct_len;

    output->header = header;
    output->header_len = 5 ;
}

void AVSRequestBuilder::buildBody( const HTTPHeader * header , const HTTPContent * content , AVSSourceData * output )
{
    stringstream build_header;
    stringstream build_body;
    stringstream build_final;
    size_t len = 0;
    char * source = nullptr;

    build_header << "--" << header->getBoundary() << std::endl;
    build_header << header->getContentDisposition() << std::endl;
    build_header << header->getContentType() << std::endl;
    build_body   << std::endl;
    build_body   << content->getContent() << std::endl;
    build_body   << std::endl;
    build_body   << "--" << header->getBoundary() << "--";
    build_body   << '\0';

    build_final << build_header.str();
    build_final << build_body.str();

    len = strlen( build_final.str().c_str() );
    source = new char[ len + 1 ];
    memset( source , 0 , len + 1);
    memcpy( source , build_final.str().c_str() , len );

    output->source     = source;
    output->source_len = len;
}

void AVSRequestBuilder::buildMultiplexBody( const HTTPRequestMultipartContent * content , size_t audio_len ,  AVSSourceData * output )
{
    stringstream multipart_json;
    stringstream multipart_audio_header;
    stringstream final_boundary;

    HTTPHeader  * mp_one_header  = content->header_one;
    HTTPHeader  * mp_two_header  = content->header_two;
    HTTPContent * mp_one_content = content->content_one;
    HTTPContent * mp_two_content = content->content_two;

    size_t multipart_json_len = 0;
    size_t multipart_audio_header_len  = 0;
    size_t multipart_audio_content_len = 0;
    size_t final_boundary_len          = 0;
    size_t final_size_of_data          = 0;

    // Multipart Json Header
    multipart_json << "--" << mp_one_header->getBoundary() << std::endl;
    multipart_json << mp_one_header->getContentDisposition() << std::endl;
    multipart_json << mp_one_header->getContentType() << std::endl;

    // Multipart Json Content
    multipart_json << std::endl;
    multipart_json << mp_one_content->getContent() << std::endl;
    multipart_json << std::endl;

    //Header
    multipart_audio_header << "--" << mp_two_header->getBoundary() << std::endl;
    multipart_audio_header << mp_two_header->getContentDisposition() << std::endl;
    multipart_audio_header << mp_two_header->getContentType() << std::endl;

    /* MULTIPART ENDE*/
    final_boundary << "--" << mp_one_header->getBoundary() << "--";

    // GET SUM OF ALL
    multipart_json_len          = strlen( multipart_json.str().c_str() );
    multipart_audio_header_len  = strlen( multipart_audio_header.str().c_str() );
    multipart_audio_content_len = audio_len + 3; // add 3 Bytes (std::endl * 3)
    final_boundary_len          = strlen( final_boundary.str().c_str() );
    final_size_of_data          = multipart_json_len + multipart_audio_header_len + multipart_audio_content_len + final_boundary_len;

    // make final source
    char * source    = new char[ final_size_of_data ];
    char * byte_endl = new char('\n');
    size_t offset    = 0;

    memset( source , 0 , final_size_of_data );
    memcpy( source + offset , multipart_json.str().c_str() , multipart_json_len );
    offset += multipart_json_len;

    memcpy( source + offset , multipart_audio_header.str().c_str() , multipart_audio_header_len );
    offset += multipart_audio_header_len;

    memcpy( source + offset , byte_endl , 1);
    offset += 1;

    memcpy( source + offset , mp_two_content->getContent() , audio_len );
    offset += audio_len;

    memcpy( source + offset , byte_endl , 1);
    offset += 1;

    memcpy( source + offset , byte_endl , 1);
    offset += 1;

    memcpy( source + offset , final_boundary.str().c_str() , final_boundary_len );

    delete byte_endl;

    output->source     = source;
    output->source_len = final_size_of_data;
}
