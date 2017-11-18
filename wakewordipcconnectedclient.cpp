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
#include "include/wakewordipcconnectedclient.h"

using namespace WakeWord;

WakeWordIPCConnectedClient::WakeWordIPCConnectedClient(int client_socket, WakeWordIPCSocket *ipc_socket)
    : Thread::Runnable() ,
      m_wakeword_ipc_socket( ipc_socket ) ,
      m_client_socket( client_socket )    ,
      m_running( true )
{}

void WakeWordIPCConnectedClient::init()
{
    //@ Starte Client Thread
    boost::thread thread(&WakeWordIPCConnectedClient::run, this );
}


void WakeWordIPCConnectedClient::run()
{
    while( m_running )
    {
        try{

            int receivedBuffer  = 0;
            int receivedCommand = 0;

            recv( m_client_socket , &receivedBuffer, 4 , 0); // Get 4 Bytes ( integer )
            receivedCommand = ntohl( receivedBuffer );

            Logger::instance().info("WakeWord Bytes received:",receivedCommand);

            if( WakeWordIPC::IPC_DISCONNECT  == receivedCommand || receivedCommand <= 0) {

                Logger::instance().info("IPC Client is Terminated");

                terminate();

            } else if( WakeWordIPC::IPC_WAKE_WORD_DETECTED == receivedCommand ) {

                Logger::instance().info("Received wake word detected");

                m_wakeword_ipc_socket->processWakeWordDetected();
            }

        }catch( ErrorException e )
        {
             Logger::instance().warn("Could not read/process the command received:", e );

             terminate();
        }
    }
}

void WakeWordIPCConnectedClient::terminate()
{
    Logger::instance().info("Terminating / Disconnecting a Wake Word Agent, Bye!");

    m_running = false;
    m_wakeword_ipc_socket->unregisterClient( this );

    try{

        close( m_client_socket );

    }catch( ErrorException e )
    {
        Logger::instance().warn("Could not close socket");
    }
}

void WakeWordIPCConnectedClient::sendCommand( IPCCommand command ) throw( std::exception )
{
    int bytes_to_send = htonl( command );
    Logger::instance().info("Send Bytes :",  ntohl( bytes_to_send ) );
    send( m_client_socket , (const char*) &bytes_to_send , 4 , 0 ); // send 4 bytes
}

