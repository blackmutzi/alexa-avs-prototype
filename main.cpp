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
#include "stdafx.h"
#include "logger.h"
#include "avsapplication.h"
#include "audiocapture.h"
#include "jsonfactory.h"
#include "audioplayback.h"
#include "http2multipartanalyse.h"
#include "avsauthenticator.h"
#include "avsconfigauthenticator.h"

// UTILITY
#include "bytefileloader.h"

int main(int argc, char **argv)
{
    Logger & log = Logger::instance();

    stringstream version_output_app;
    stringstream version_output_nghttp2;
    stringstream version_output_boost;

    version_output_app << "C++ AVS-Prototype v.1.0";
    version_output_nghttp2 << "NgHTTP2 " << nghttp2_version(0)->version_str;
    version_output_boost << "Boost " << BOOST_VERSION;

    log.info( version_output_app.str() );
    log.info( version_output_nghttp2.str() );
    log.info( version_output_boost.str() );

    AVSApplication application;
    application.main( argc , argv );

    return 0;
}

