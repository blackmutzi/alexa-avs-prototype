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
#ifndef STDAFX_H
#define STDAFX_H

#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#endif

/* Other Header */
#include <fstream>
#include <iostream>
#include <istream>
#include <ios>
#include <exception>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <sstream>
#include <mutex>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/mman.h>

/* C Header */
#include <ctime>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>

/* Socket */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/* ALSA */
#include <alsa/asoundlib.h>

/* curl */
#include <curl/curl.h>

/* OpenSSL & nghttp2 */
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>

#include <event.h>
#include <event2/event.h>
#include <event2/bufferevent_ssl.h>
#include <event2/dns.h>

#include <nghttp2/nghttp2.h>

/* Boost */
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

/* LAME / MPEG DECODER */
#include <lame/lame.h>


// 1 Byte
//using uint8_t  = unsigned char;
// 2 Bytes
//using uint16_t = unsigned short;
// 4 Bytes
//using uint32_t = unsigned int;
// 8 Bytes
//using uint64_t = unsigned long long;

using namespace std;

/* Exceptions */

struct ErrorException : public std::exception
{
   std::string s;
   ErrorException(std::string ss) : s(ss) {}
   ErrorException(std::string ss , const char * c_ss ){
       stringstream stream;
       stream << ss << " " << c_ss;
       s = stream.str();
   }

   ~ErrorException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};

#endif // STDAFX_H
