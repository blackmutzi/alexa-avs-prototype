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
#include "include/httpheader.h"

using namespace Network::HTTP;

std::string HTTPHeader::HTTP_HEADERMSG_CONTENT_DISPOSITION = "Content-Disposition: ";
std::string HTTPHeader::HTTP_HEADERMSG_CONTENT_TYPE = "Content-Type: ";

HTTPHeader::HTTPHeader(const string boundary)
    : m_content_disposition("") , m_content_type("") , m_boundary(boundary)
{}

HTTPHeader::HTTPHeader(const string content_disposition, const string content_type)
    : m_content_disposition(content_disposition) , m_content_type( content_type ) , m_boundary("")
{}

void HTTPHeader::setContentDisposition(const string content_disposition)
{
    m_content_disposition = content_disposition;
}

void HTTPHeader::setContentType(const string content_type)
{
    m_content_type = content_type;
}

void HTTPHeader::setBoundary(const string boundary)
{
    m_boundary = boundary;
}

const std::string & HTTPHeader::getContentDisposition(void) const
{
    return m_content_disposition;
}

const std::string & HTTPHeader::getContentType(void) const
{
    return m_content_type;
}

const std::string & HTTPHeader::getBoundary(void) const
{
    return m_boundary;
}
