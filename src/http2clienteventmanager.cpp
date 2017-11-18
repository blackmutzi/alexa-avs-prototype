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
#include "include/http2clienteventmanager.h"

using namespace Network::HTTP;

HTTP2ClientEventManager::HTTP2ClientEventManager()
    : m_stream_status( new int( -1 ) ) ,
      m_downchannel_status( false )
{}

HTTP2ClientEventManager::~HTTP2ClientEventManager()
{
    delete m_stream_status;
}

void HTTP2ClientEventManager::registerDirectivenListener(DirectivenListener *listener)
{
    m_directiven_listener_list.push_back( listener );
}

void HTTP2ClientEventManager::unregisterDirectivenListener(DirectivenListener *listener)
{
    m_directiven_listener_list.remove( listener );
}


void HTTP2ClientEventManager::registerAudioDirectivenListener(AudioPlayerDirectivenListener *listener)
{
    m_audio_directiven_listener_list.push_back( listener );
}

void HTTP2ClientEventManager::unregisterAudioDirectivenListener(AudioPlayerDirectivenListener *listener)
{
    m_audio_directiven_listener_list.remove( listener );
}

void HTTP2ClientEventManager::setDownChannelStatus(bool status)
{
    m_downchannel_status = status;
}

bool HTTP2ClientEventManager::getDownChannelStatus(void)
{
    return m_downchannel_status;
}

int * HTTP2ClientEventManager::getStreamStatus(void)
{
    return m_stream_status;
}

void HTTP2ClientEventManager::setBoundary( std::string boundary )
{
    m_boundary = boundary;
}

HTTPClientDirectiveEvent * HTTP2ClientEventManager::http_response_analyse( HTTPResponse * response )
{
    HTTPClientDirectiveEvent * event = new HTTPClientDirectiveEvent();
    HTTP2MultipartAnalyse * analyse  = new HTTP2MultipartAnalyse();

    analyse->analyse( (char*) response->getContent() , response->getSize() );
    analyse->update( event );

    delete analyse;
    return event;
}

void HTTP2ClientEventManager::invokeAllEvents( HTTPResponse * response )
{
    //GET Directive Events
    HTTPClientDirectiveEvent * event = http_response_analyse( response );
    std::string directive_namespace  = "";
    std::string directive_name       = "";
    JsonObject * object              = nullptr;

    for( unsigned int json_count =0; json_count < event->getJsonObjectCount(); json_count++)
    {
        // GET Json object
        object = event->getJsonObject( json_count );

        // Analyse Json object
        try
        {
            Logger::instance().info( object->json_string );

            JsonReaderDirective * json_reader = new JsonReaderDirective();
            json_reader->setJsonContent( object->json_string  );
            directive_namespace = json_reader->getHeaderNameSpace();
            directive_name      = json_reader->getHeaderName();
            delete json_reader;

            Logger::instance().info( object->json_string );

        }catch( std::exception e )
        {
            Logger::instance().warn("Bad Json Object found");
            break;
        }

        for( DirectivenListener * listener : m_directiven_listener_list )
        {
            if( directive_namespace == AVSJson::SpeechRecognizer::NAME_SPACE )
            {
                if( directive_name == AVSJson::SpeechRecognizer::DIRECTIVE_STOP_CAPTURE )
                            listener->onSpeechRecognizeStopCaptureDirective( event );

                if( directive_name == AVSJson::SpeechRecognizer::DIRECTIVE_EXPECT_SPEECH )
                            listener->onSpeechRecognizeExpectSpeechDirective( event );
            }

            if( directive_namespace == AVSJson::SpeechSynthesizer::NAME_SPACE )
            {
                if( directive_name == AVSJson::SpeechSynthesizer::DIRECTIVE_SPEAK )
                            listener->onSpeechSynthesizerSpeakDirective( event );
            }

            if( directive_namespace == AVSJson::Speaker::NAME_SPACE )
            {
                if( directive_name == AVSJson::Speaker::DIRECTIVE_SET_MUTE )
                            listener->onSpeekerSetMuteDirective( event );
            }
        }

        for( AudioPlayerDirectivenListener * listener : m_audio_directiven_listener_list )
        {
            if( directive_namespace == AVSJson::AudioPlayer::NAME_SPACE )
            {
                if( directive_name == AVSJson::AudioPlayer::DIRECTIVE_PLAY )
                {
                     listener->onPlayDirective( event );
                }

                if( directive_name == AVSJson::AudioPlayer::DIRECTIVE_STOP )
                {
                    listener->onStopDirective( event );
                }

                if( directive_name == AVSJson::AudioPlayer::DIRECTIVE_CLEAR_QUEUE )
                {
                    listener->onClearQueueDirective( event );
                }
            }
        }
    }

    delete event;
}
 
