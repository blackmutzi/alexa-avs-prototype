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
#include "include/httpheaderfactory.h"

using namespace Network::HTTP;


std::string HTTPHeaderFactory::EVENTS_ENDPOINT = "/v20160207/events";
std::string HTTPHeaderFactory::DIRECTIVES_ENDPOINT = "/v20160207/directives";
std::string HTTPHeaderFactory::PING_ENDPOINT = "/ping";

HTTPHeaderFactory::HTTPHeaderFactory(){}

HTTP2Header HTTPHeaderFactory::create_HTTP2_POST(const string access_token, const string event_or_directive, const std::string boundary )
{
    HTTP2Header header;

    stringstream build_authorization;
    build_authorization << "Bearer " << access_token;

    stringstream build_content_type;
    build_content_type << "multipart/form-data;" << " boundary=" << boundary;

    header.setMethode("POST");
    header.setSheme("https");
    header.setPath( event_or_directive );
    header.setAuthorization( build_authorization.str());
    header.setContentType( build_content_type.str() );
    header.setBoundary(boundary);

    return header;
}

HTTP2Header HTTPHeaderFactory::create_HTTP2_GET(const string access_token, const string event_or_directive , const std::string boundary )
{
    HTTP2Header header;

    stringstream build_authorization;
    build_authorization << "Bearer " << access_token;

    stringstream build_content_type;
    build_content_type << "multipart/form-data;" << " boundary=" << boundary;

    header.setMethode("GET");
    header.setSheme("https");
    header.setPath( event_or_directive );
    header.setAuthorization( build_authorization.str());
    header.setContentType( build_content_type.str() );
    header.setBoundary(boundary);

    return header;
}

HTTPHeader HTTPHeaderFactory::createJSONHeader( const std::string boundary )
{
    HTTPHeader header (boundary);

    stringstream build_content_disposition;
    build_content_disposition << HTTPHeader::HTTP_HEADERMSG_CONTENT_DISPOSITION << "form-data; name=\"metadata\"";
    header.setContentDisposition( build_content_disposition.str() );

    stringstream build_content_type;
    build_content_type << HTTPHeader::HTTP_HEADERMSG_CONTENT_TYPE << "application/json; charset=UTF-8";
    header.setContentType( build_content_type.str() );

    return header;
}

HTTPHeader HTTPHeaderFactory::createAudioHeader(const string boundary)
{
    HTTPHeader header ( boundary );

    stringstream build_content_disposition;
    build_content_disposition << HTTPHeader::HTTP_HEADERMSG_CONTENT_DISPOSITION << "form-data; name=\"audio\"";
    header.setContentDisposition( build_content_disposition.str() );

    stringstream build_content_type;
    build_content_type << HTTPHeader::HTTP_HEADERMSG_CONTENT_TYPE << "application/octet-stream";
    header.setContentType( build_content_type.str() );

    return header;
}
