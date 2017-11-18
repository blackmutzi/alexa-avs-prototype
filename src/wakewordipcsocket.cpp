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
#include "include/wakewordipcsocket.h"

using namespace WakeWord;

Logger & WakeWordIPCSocket::log = Logger::instance();

WakeWordIPCSocket::WakeWordIPCSocket(WakeWordDetectedHandler *handler, int portNumber) throw( std::exception )
    : WakeWordIPC( handler )
{
    m_serverSocket = new Network::NetSocket::ServerSocket( portNumber );
}

void WakeWordIPCSocket::init()
{
    //@ Starte Server Thread

    log.info("init WakeWordIPCSocket");
    boost::thread thread(&WakeWordIPCSocket::run, this );;
}

void WakeWordIPCSocket::sendCommand(IPCCommand ipc_command ) throw( std::exception )
{
    //@TODO - LogFactory

    stringstream debug_info;
    debug_info << "Sending Command ... " << WakeWordIPC::IPCCommandNames[ ipc_command ];
    debug_info << " to all connected clients " << std::endl;

    log.debug( debug_info.str() );

    for( WakeWordIPCConnectedClient * client : m_connectedClients )
    {
        client->sendCommand( ipc_command );
    }
}

void WakeWordIPCSocket::run()
{
    while(true)
    {
        try{

            int clientSocket = m_serverSocket->socketAccept();

            WakeWordIPCConnectedClient * newConnectedClient = new WakeWordIPCConnectedClient( clientSocket , this );

            newConnectedClient->init();

            registerClient( newConnectedClient );


        }catch( ErrorException e )
        {
            log.warn("Could not accept/connect IPC client", e);
        }
    }
}

void WakeWordIPCSocket::registerClient( WakeWordIPCConnectedClient * client )
{
    log.info("New IPC Client was accepted");

    m_connectedClients.push_back( client );
}


void WakeWordIPCSocket::unregisterClient(WakeWordIPCConnectedClient *old_client)
{
    log.info("IPC client was removed");

    m_connectedClients.remove( old_client );
    delete old_client;
}

void WakeWordIPCSocket::processWakeWordDetected()
{
    log.info("Wake Word Detected ...");

    WakeWordIPC::wakeWordDetected();
}











