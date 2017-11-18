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
#include "include/avsapplication.h"

AVSApplication::AVSApplication()
    : WakeWordDetectedHandler() ,

      m_ipc_factory( new WakeWordIPCFactory() ) ,
      m_avs_controller( nullptr ) ,
      m_avs_client( nullptr ) ,
      m_started( false )
{}

AVSApplication::~AVSApplication()
{
    delete m_ipc_factory;

    if( m_started )
    {
        delete m_avs_controller;
        delete m_avs_client;
    }
}

void AVSApplication::main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    #ifdef LOGGER_DEBUG
        Logger::instance().info("Debug Mode Activ");
    #endif
   m_avs_controller = new AVSController( m_ipc_factory , this );
   m_avs_controller->startWakeWordServer();
   m_avs_controller->startAVSClient();
   m_avs_controller->startAudioStreaming();


   m_avs_client  = m_avs_controller->getAVSClient();
   m_started = true;
   std::string restart_access_token = "";

   while( m_started )
   {
      if( ! m_avs_client->getEventManager()->getDownChannelStatus() )     // if downchannel down
      {
            Logger::instance().info("[RESTART MODUL] Generate new Access Token");

            restart_access_token = m_avs_controller->getAuthenticator()->GenerateAccessToken( m_avs_controller->getAuthenticator()->getRefreshToken() );

            Logger::instance().info("[RESTART MODUL] Restart AVS Client ");
            m_avs_client->changeAccessToken( restart_access_token );
            m_avs_client->restart();

      } else {

          // downchannel is up

          // main loop is in sleep mode
          boost::this_thread::sleep(boost::posix_time::seconds(5));
      }
   }
}

void AVSApplication::onWakeWordDetected(void)
{
    //@Alexa WakeWord Detected
    m_avs_controller->startAudioStreaming();
}
