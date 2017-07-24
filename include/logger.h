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
#ifndef LOGGER_H
#define LOGGER_H

#include "stdafx.h"

class Logger
{
    public:

        static const std::string kLogLevelDebug;
        static const std::string kLogLevelInfo;
        static const std::string kLogLevelWarning;
        static const std::string kLogLevelError;

        static Logger & instance();
        static Logger & instance( const std::string & class_name );

        void log( const std::string & inLogLevel , const std::string & inMessage );

        void info ( const std::string & inMessage );
        void info ( const std::string & inMessage , const std::string & extendMessage );
        void info ( const std::string & inMessage , int integer );

        void warn ( const std::string & inMessage );
        void warn ( ErrorException e );
        void warn ( const std::string & inMessage , ErrorException e );

        void debug( const std::string & inMessage );

        void error( const std::string & inMessage );
        void error( ErrorException e );
        void error( const std::string & inMessage , ErrorException e );

    protected:

        static Logger * pInstance;

        static const char * const kLogFileName;

        std::ofstream m_output_stream;

        friend class Cleanup;
        class Cleanup
        {
            public:
            ~Cleanup();
        };

        void logHelper( const std::string & inLogLevel , const std::string & inMessage );

    private:

        Logger();
        virtual ~Logger();
        Logger(const Logger &);
        Logger& operator =( const Logger& );

        static std::mutex sMutex;
};

#endif // LOGGER_H
