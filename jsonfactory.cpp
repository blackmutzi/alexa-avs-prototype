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
#include "include/jsonfactory.h"

using namespace AVSJson;

JsonFactory::JsonFactory(){}

const std::string JsonFactory::createAlertsContext(const AlertsState *state)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("AlertContext.json");

    JsonFileLoader loader( json_file );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createPlaybackContext(const PlaybackState *state)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("PlaybackContext.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{TOKEN_STRING}}", state->token );
    loader.addReplaceData("{{MS}}", loader.convertTo( state->offsetInMilliseconds ) );
    loader.addReplaceData("{{ACTIVITY_STRING}}", state->playerActivity );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeakerContext( const VolumeState *state)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeakerContext.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{VOLUME}}", loader.convertTo( state->volume ) );
    loader.addReplaceData("{{MUTED}}" , state->muted );;

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeechSynthesizerContext( const SpeechState *state)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeechSynthesizerContext.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{TOKEN_STRING}}", state->token );
    loader.addReplaceData("{{MS}}", loader.convertTo( state->offsetInMilliseconds ) );
    loader.addReplaceData("{{ACTIVITY_STRING}}" , state->playerActivity );;

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeechRecognizerContext( const RecognizeState * state)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeechRecognizeContext.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{WAKEWORD_STRING}}", state->wakeword );

    loader.load();

    return loader.getJsonObject();
}


const std::string JsonFactory::createSystemSynchronizeEvent( const SynchronizeStateEvent * event )
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SystemSynchronize.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{Alerts.AlertsState}}"               ,  createAlertsContext( event->alertsState ) );
    loader.addReplaceData("{{AudioPlayer.PlaybackState}}"        ,  createPlaybackContext( event->playbackState ));
    loader.addReplaceData("{{Speaker.VolumeState}}"              ,  createSpeakerContext( event->volumeState ));
    loader.addReplaceData("{{SpeechSynthesizer.SpeechState}}"    ,  createSpeechSynthesizerContext( event->speechState) );
    loader.addReplaceData("{{SpeechRecognizer.RecognizerState}}" ,  createSpeechRecognizerContext( event->recognizeState));

    loader.addReplaceData("{{MESSAGE_ID_STRING}}" , event->messageId );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeechRecognizerEvent( const SynchronizeStateEvent * event , const std::string & dialog_request_id )
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeechRecognizeEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{Alerts.AlertsState}}"               ,  createAlertsContext( event->alertsState ) );
    loader.addReplaceData("{{AudioPlayer.PlaybackState}}"        ,  createPlaybackContext( event->playbackState ));
    loader.addReplaceData("{{Speaker.VolumeState}}"              ,  createSpeakerContext( event->volumeState ));
    loader.addReplaceData("{{SpeechSynthesizer.SpeechState}}"    ,  createSpeechSynthesizerContext( event->speechState) );
    loader.addReplaceData("{{SpeechRecognizer.RecognizerState}}" ,  createSpeechRecognizerContext( event->recognizeState));

    loader.addReplaceData("{{MESSAGE_ID_STRING}}" , event->messageId  );
    loader.addReplaceData("{{DIALOG_STRING}}"     , dialog_request_id );

    loader.addReplaceData("{{PROFILE_STRING}}" , AVSJson::SpeechRecognizer::PROFILE_NEAR_FIELD );
    loader.addReplaceData("{{FORMAT_STRING}}"  , AVSJson::SpeechRecognizer::AUDIO_FORMAT );

    //@TODO - AVS has was changed here ...
    loader.addReplaceData("{{TYPE_STRING}}", "TAP" );
    loader.addReplaceData("{{LONG}}", loader.convertTo( (long) 0));

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeechSynthesizerStartEvent( const std::string & messageId , const std::string & token )
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeechStartedEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{TOKEN_STRING}}" , token );

    loader.load();

    return loader.getJsonObject();
}


const std::string JsonFactory::createSpeechSynthesizerFinishEvent(const string &messageId, const string &token)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeechFinishedEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{TOKEN_STRING}}" , token );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createAlertCustomEvent(const string & event, const string &messageId, const string &token)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("AlertEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{EVENT_NAME}}" , event );
    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{TOKEN_STRING}}" , token );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createPlaybackCustomEvent(const string &event, const string &messageId, const string &token, long offset)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("PlaybackEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{EVENT_NAME}}" , event );
    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{TOKEN_STRING}}" , token );
    loader.addReplaceData("{{OFFSET_MS}}" , loader.convertTo( offset) );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createPlaybackFailedEvent( const std::string & messageId , const PayloadPlaybackFailedParam *param )
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("PlaybackFailedEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{MESSAGE_ID_STRING}}"    , messageId );
    loader.addReplaceData("{{PAYLOAD_TOKEN_STRING}}" , param->token );
    loader.addReplaceData("{{STATE_TOKEN_STRING}}"   , param->playbackState_token );
    loader.addReplaceData("{{OFFSET_MS}}"            , param->playbackState_offset);
    loader.addReplaceData("{{ACTIVITY_STRING}}"      , param->playbackState_activity);

    loader.addReplaceData("{{ERROR_TYPE_STRING}}"    , param->error_type   );
    loader.addReplaceData("{{ERROR_MSG_STRING}}"     , param->error_message);

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createPlaybackQueueClearedEvent(const string &messageId)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("PlaybackQueueClearedEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createCommandCustomEvent(const SynchronizeStateEvent * event , const string &event_name , const string &messageId)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("PlaybackControllerEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{Alerts.AlertsState}}"               ,  createAlertsContext( event->alertsState ) );
    loader.addReplaceData("{{AudioPlayer.PlaybackState}}"        ,  createPlaybackContext( event->playbackState ));
    loader.addReplaceData("{{Speaker.VolumeState}}"              ,  createSpeakerContext( event->volumeState ));
    loader.addReplaceData("{{SpeechSynthesizer.SpeechState}}"    ,  createSpeechSynthesizerContext( event->speechState) );
    loader.addReplaceData("{{SpeechRecognizer.RecognizerState}}" ,  createSpeechRecognizerContext( event->recognizeState));

    loader.addReplaceData("{{MESSAGE_ID_STRING}}" , messageId );
    loader.addReplaceData("{{EVENT_NAME}}" , event_name );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSpeakerCustomEvent(const string &event, const string &messageId, long volume, bool muted)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SpeakerEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{EVENT_NAME}}" , event );
    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{VOLUME}}" , loader.convertTo( volume ) );
    loader.addReplaceData("{{MUTED}}"  , loader.convertTo( muted  ) );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createSettingsUpdateEvent(const string &messageId, const string &key, const string &value)
{
    std::string json_file;
    json_file.append( AVSJson::JSON_RESOURCE_ASSET_PATH );
    json_file.append("SettingsUpdateEvent.json");

    JsonFileLoader loader( json_file );

    loader.addReplaceData("{{MESSAGE_ID_STRING}}", messageId );
    loader.addReplaceData("{{KEY_STRING}}"    , key   );
    loader.addReplaceData("{{VALUE_STRING}}"  , value );

    loader.load();

    return loader.getJsonObject();
}

const std::string JsonFactory::createVolumeChangedEvent(const string &messageId, long volume, bool muted)
{
    return createSpeakerCustomEvent( AVSJson::Speaker::HEADER_VOLUME_CHANGED , messageId ,volume , muted );
}

const std::string JsonFactory::createMuteChangedEvent(const string &messageId, long volume, bool muted)
{
    return createSpeakerCustomEvent( AVSJson::Speaker::HEADER_MUTE_CHANGED , messageId , volume , muted );
}

const std::string JsonFactory::createPlayCommandIssuedEvent(const SynchronizeStateEvent *event , const string &messageId )
{
    return createCommandCustomEvent( event , AVSJson::PlaybackController::HEADER_PLAY_COMMAND_ISSUED , messageId );
}

const std::string JsonFactory::createPauseCommandIssuedEvent(const SynchronizeStateEvent *event, const string &messageId)
{
    return createCommandCustomEvent( event , AVSJson::PlaybackController::HEADER_PAUSE_COMMAND_ISSUED , messageId );
}

const std::string JsonFactory::createNextCommandIssuedEvent( const SynchronizeStateEvent * event ,const string & messageId )
{
    return createCommandCustomEvent( event , AVSJson::PlaybackController::HEADER_NEXT_COMMAND_ISSUED , messageId );
}

const std::string JsonFactory::createPreviousCommandIssuedEvent( const SynchronizeStateEvent * event , const string & messageId )
{
    return createCommandCustomEvent( event , AVSJson::PlaybackController::HEADER_PREVIOUS_COMMAND_ISSUED , messageId );
}

const std::string JsonFactory::createPlaybackStartedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_STARTED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackNearlyFinishedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_NEARLY_FINISHED , messageId , token , offset );
}

const std::string JsonFactory::createProgressReportDelayElapsedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_PROGRESS_REPORT_DELAY_ELAPSED , messageId , token , offset );
}

const std::string JsonFactory::createProgressReportIntervalElapsedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_PROGRESS_REPORT_INTERVAL_ELAPSED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackStutterStartedEvent( const string & messageId , const std::string & token , long offset )
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_STUTTER_STARTED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackStutterFinishedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_STUTTER_FINISHED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackFinishedEvent(const string &messageId, const string &token, long offset)
{
    //@TODO ...
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_FINISHED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackPausedEvent( const string &messageId , const string &token , long offset )
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_PAUSED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackResumedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_RESUMED , messageId , token , offset );
}

const std::string JsonFactory::createPlaybackStoppedEvent(const string &messageId, const string &token, long offset)
{
    return createPlaybackCustomEvent( AVSJson::AudioPlayer::HEADER_PLAYBACK_STOPPED , messageId , token , offset );
}

const std::string JsonFactory::createSetAlertSucceededEvent(const string &messageId, const string &token)
{
    return createAlertCustomEvent( AVSJson::Alerts::HEADER_SET_ALERT_SUCCEEDED , messageId , token );
}

const std::string JsonFactory::createSetAlertFailedEvent(const string &messageId, const string &token)
{
    return createAlertCustomEvent( AVSJson::Alerts::HEADER_SET_ALERT_FAILED , messageId , token );
}

const std::string JsonFactory::createDeleteAlertFailedEvent(const string &messageId, const string &token)
{
    return createAlertCustomEvent( AVSJson::Alerts::HEADER_DELETE_ALERT_FAILED , messageId , token );
}

const std::string JsonFactory::createAlertStartedEvent(const string &messageId, const string &token)
{
   return createAlertCustomEvent(  AVSJson::Alerts::HEADER_ALERT_STARTED , messageId , token  );
}

const std::string JsonFactory::createAlertStoppedEvent(const string &messageId, const string &token)
{
   return createAlertCustomEvent( AVSJson::Alerts::HEADER_ALERT_STOPPED , messageId , token );
}

const std::string JsonFactory::createAlertEnteredForegroundEvent(const string &messageId, const string &token)
{
   return createAlertCustomEvent( AVSJson::Alerts::HEADER_ALERT_ENTERED_FOREGROUND , messageId , token );
}

const std::string JsonFactory::createAlertEnteredBackgroundEvent( const string &messageId , const string & token )
{
    return createAlertCustomEvent( AVSJson::Alerts::HEADER_ALERT_ENTERED_BACKGROUND , messageId , token );
}
 
