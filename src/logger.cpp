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
#include "include/logger.h"

const string Logger::kLogLevelDebug   = "DEBUG";
const string Logger::kLogLevelInfo    = "INFO";
const string Logger::kLogLevelWarning = "WARNING";
const string Logger::kLogLevelError   = "ERROR";

const char* const Logger::kLogFileName = "application.log";

Logger* Logger::pInstance = nullptr;

mutex Logger::sMutex;

Logger& Logger::instance()
{
    static Cleanup cleanup;

    lock_guard<mutex> guard(sMutex);

    if (pInstance == nullptr)
               pInstance = new Logger();

    return *pInstance;
}

Logger::Cleanup::~Cleanup()
{
    lock_guard<mutex> guard(Logger::sMutex);

    delete Logger::pInstance;

    Logger::pInstance = nullptr;
}

Logger::~Logger()
{
    m_output_stream.close();
}

Logger::Logger()
{
    m_output_stream.open(kLogFileName, ios_base::app);

    if (!m_output_stream.good()) {
        throw runtime_error("Unable to initialize the Logger!");
    }
}

void Logger::info(const string &inMessage, int integer )
{
    stringstream stream;
    stream << inMessage << " " << integer;

    info( stream.str() );
}

void Logger::info( const string & inMessage , const string & extendMessage )
{
    stringstream stream;
    stream << inMessage << "  " << extendMessage;

    info( stream.str() );
}

void Logger::warn(const string &inMessage, ErrorException e )
{
    stringstream stream;
    stream << inMessage << " " << e.what();

    warn( stream.str() );
}

void Logger::error(const string &inMessage, ErrorException e)
{
    stringstream stream;
    stream << inMessage << " " << e.what();

    error( stream.str() );
}

void Logger::warn( ErrorException e )
{
    stringstream stream;
    stream << e.what();

    warn( stream.str() );
}

void Logger::error( ErrorException e )
{
    stringstream stream;
    stream << e.what();

    error( stream.str() );
}

void Logger::info(const string &inMessage)
{
    log( kLogLevelInfo , inMessage );
}

void Logger::warn( const string & inMessage )
{
    log( kLogLevelWarning , inMessage );
}

void Logger::debug( const string & inMessage )
{
    #ifdef LOGGER_DEBUG
        log( kLogLevelDebug , inMessage );
    #endif
}

void Logger::error( const string & inMessage )
{
    log( kLogLevelError , inMessage );

    exit(1);
}

void Logger::log(const string& inLogLevel , const string& inMessage )
{
    lock_guard<mutex> guard(sMutex);

    logHelper( inLogLevel , inMessage );
}


void Logger::logHelper(const std::string& inLogLevel , const std::string& inMessage )
{
    m_output_stream << inLogLevel << ": " << inMessage << endl;

    std::cout << inLogLevel << ": " << inMessage << endl;
}
