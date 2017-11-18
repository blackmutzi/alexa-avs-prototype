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
#ifndef FILELOADER_H
#define FILELOADER_H

#include "stdafx.h"

class FileLoader
{
    public:
        FileLoader();
        FileLoader( const std::string file_name );

        void setFileName( const std::string file_name );

        void load();

        virtual void onLineEvent( const std::string line ) =0;

        virtual void onEndOfFileEvent( void ) =0;

        void replaceAll( std::string & input , std::string search , std::string replace );

        std::string convertTo( long number  );
        std::string convertTo( bool boolean );

    private:

        std::string m_file_name;
};

#endif // FILELOADER_H
