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
#ifndef JSON_H
#define JSON_H

#include "stdafx.h"
#include "nlohmann_json.h"
#include "jsonfileloader.h"

namespace AVSJson
{
     class JsonFactory;
     class JsonReaderDirective;
     class JsonFileLoader;

     using json = nlohmann::json;

     static std::string JSON_RESOURCE_ASSET_PATH = "./asset/";

     static std::string JSON_EVENT     = "event";
     static std::string JSON_DIRECTIVE = "directive";
     static std::string JSON_HEADER    = "header";
     static std::string JSON_PAYLOAD   = "payload";
     static std::string JSON_NAMESPACE = "namespace";
     static std::string JSON_NAME      = "name";
     static std::string JSON_CONTEXT   = "context";
     static std::string JSON_ERROR     = "error";

     struct AlertsState;
     struct VolumeState;
     struct SpeechState;
     struct PlaybackState;
     struct RecognizeState;
     struct SynchronizeStateEvent;
     struct PayloadPlaybackFailedParam;


     //System.SynchronizeState
     namespace System
     {
             static std::string NAME_SPACE = "System";
             static std::string HEADER_SYNCHRONIZE_STATE_NAME      = "SynchronizeState";
             static std::string HEADER_USER_INACTIVITY_REPORT_NAME = "UserInactivityReport";
             static std::string HEADER_PARAM_MESSAGEID             = "messageId";

             static std::string PAYLOAD_PARAM_INACTIVE_TIME_IN_SECONDS = "inactiveTimeInSeconds";
     }

     namespace Settings
     {
             static std::string NAME_SPACE = "Settings";
             static std::string HEADER_SETTINGS_UPDATED = "SettingsUpdated";
             static std::string HEADER_PARAM_MESSAGE_ID = "messageId";

             static std::string PAYLOAD_SETTINGS = "settings";
             static std::string PAYLOAD_SETTINGS_KEY = "key";
             static std::string PAYLOAD_SETTINGS_VALUE= "value";

             static std::string ACCEPT_KEYS_LOCALE = "locale";
             static std::string ACCEPT_VALUE_US    = "en-US";
             static std::string ACCEPT_VALUE_GB    = "en-GB";
             static std::string ACCEPT_VALUE_DE    = "de-DE";
     }

     namespace PlaybackController
     {
            static std::string NAME_SPACE = "PlaybackController";
            static std::string HEADER_PLAY_COMMAND_ISSUED     = "PlayCommandIssued";
            static std::string HEADER_PAUSE_COMMAND_ISSUED    = "PauseCommandIssued";
            static std::string HEADER_NEXT_COMMAND_ISSUED     = "NextCommandIssued";
            static std::string HEADER_PREVIOUS_COMMAND_ISSUED = "PreviousCommandIssued";

            static std::string HEADER_PARAM_MESSAGE_ID    = "messageId";
     }

     //AudioPlayer.PlaybackState
     namespace AudioPlayer
     {
            static std::string NAME_SPACE = "AudioPlayer";
            static std::string HEADER_PLAYBACK_STATE_NAME      = "PlaybackState";
            static std::string HEADER_PLAYBACK_STARTED         = "PlaybackStarted";
            static std::string HEADER_PLAYBACK_NEARLY_FINISHED = "PlaybackNearlyFinished";
            static std::string HEADER_PLAYBACK_PROGRESS_REPORT_DELAY_ELAPSED = "ProgressReportDelayElapsed";
            static std::string HEADER_PLAYBACK_PROGRESS_REPORT_INTERVAL_ELAPSED = "ProgressReportIntervalElapsed";
            static std::string HEADER_PLAYBACK_STUTTER_STARTED  = "PlaybackStutterStarted";
            static std::string HEADER_PLAYBACK_STUTTER_FINISHED = "PlaybackStutterFinished";
            static std::string HEADER_PLAYBACK_FINISHED         = "PlaybackFinished";
            static std::string HEADER_PLAYBACK_FAILED           = "PlaybackFailed";
            static std::string HEADER_PLAYBACK_STOPPED          = "PlaybackStopped";
            static std::string HEADER_PLAYBACK_PAUSED           = "PlaybackPaused";
            static std::string HEADER_PLAYBACK_RESUMED          = "PlaybackResumed";
            static std::string HEADER_PLAYBACK_QUEUE_CLEARED    = "PlaybackQueueCleared";
            static std::string HEADER_STREAM_METADATA_EXTRACTED = "StreamMetadataExtracted";

            static std::string HEADER_MESSAGE_ID                = "messageId";

            static std::string PAYLOAD_PARAM_TOKEN = "token";
            static std::string PAYLOAD_PARAM_OFFSET_IN_MILLISECONDS = "offsetInMilliseconds";
            static std::string PAYLOAD_PARAM_PLAYER_ACTIVITY = "playerActivity";
            static std::string PAYLOAD_PARAM_STUTTER_DURATION_MS = "stutterDurationMilliseconds";
            static std::string PAYLOAD_CURRENT_PLAYBACK_STATE    = "currentPlaybackState";
            static std::string PAYLOAD_PARAM_METADATA            = "metadata";

            static std::string ERROR_TYPE = "type";
            static std::string ERROR_MESSAGE = "message";

            static std::string PLAYER_ACTIVITY_IDLE    = "IDLE";
            static std::string PLAYER_ACTIVITY_PLAYING = "PLAYING";
            static std::string PLAYER_ACTIVITY_PAUSED  = "PAUSED";
            static std::string PLAYER_ACTIVITY_BUFFER_UNDERRUN = "BUFFER_UNDERRUN";
            static std::string PLAYER_ACTIVITY_FINISHED = "FINISHED";
            static std::string PLAYER_ACTIVITY_STOPPED  = "STOPPED";
     }

     //Alerts.AlertsState
     namespace Alerts
     {
            static std::string NAME_SPACE = "Alerts";
            static std::string HEADER_ALERTS_STATE_NAME = "AlertsState";
            static std::string HEADER_SET_ALERT_SUCCEEDED = "SetAlertSucceeded";
            static std::string HEADER_SET_ALERT_FAILED    = "SetAlertFailed";
            static std::string HEADER_DELETE_ALERT_FAILED = "DeleteAlertFailed";
            static std::string HEADER_ALERT_STARTED       = "AlertStarted";
            static std::string HEADER_ALERT_STOPPED       = "AlertStopped";
            static std::string HEADER_ALERT_ENTERED_FOREGROUND = "AlertEnteredForeground";
            static std::string HEADER_ALERT_ENTERED_BACKGROUND = "AlertEnteredBackground";
            static std::string HEADER_MESSAGE_ID          = "messageId";

            static std::string PAYLOAD_PARAM_ALL_ALERTS = "allAlerts";
            static std::string PAYLOAD_PARAM_ACTIVE_ALERTS = "activeAlerts";
            static std::string PAYLOAD_PARAM_TOKEN         = "token";

            static std::string ALL_ALERTS_TOKEN = "token";
            static std::string ALL_ALERTS_TYPE  = "type";
            static std::string ALL_ALERTS_SCHEDULEDTIME = "scheduledTime";

            static std::string ALERTS_TYPE_TIMER = "TIMER";
            static std::string ALERTS_TYPE_ALARM = "ALARM";
     }


     //Speaker.VolumeState
     namespace Speaker
     {
            static std::string NAME_SPACE = "Speaker";
            static std::string HEADER_VOLUME_STATE_NAME = "VolumeState";
            static std::string HEADER_VOLUME_CHANGED    = "VolumeChange";
            static std::string HEADER_MUTE_CHANGED      = "MuteChanged";
            static std::string HEADER_MESSAGE_ID        = "messageId";

            static std::string DIRECTIVE_SET_MUTE       = "SetMute";

            static std::string PAYLOAD_PARAM_VOLUME = "volume";
            static std::string PAYLOAD_PARAM_MUTED  = "muted";
            static std::string PAYLOAD_PARAM_MUTE   = "mute";
     }


     //SpeechSynthesizer.SpeechState
     namespace SpeechSynthesizer
     {
            static std::string NAME_SPACE = "SpeechSynthesizer";
            static std::string HEADER_SPEECH_STATE_NAME = "SpeechState";
            static std::string HEADER_SPEECH_STARTED    = "SpeechStarted";
            static std::string HEADER_SPEECH_FINISHED   = "SpeechFinished";
            static std::string HEADER_MESSAGE_ID        = "messageId";

            static std::string PAYLOAD_PARAM_TOKEN = "token";
            static std::string PAYLOAD_PARAM_OFFSET_IN_MILLISECONDS = "offsetInMilliseconds";
            static std::string PAYLOAD_PARAM_PLAYER_ACTIVITY = "playerActivity";

            static std::string PLAYER_ACTIVITY_PLAYING = "PLAYING";
            static std::string PLAYER_ACTIVITY_FINISHED = "FINISHED";

            static std::string DIRECTIVE_SPEAK = "Speak";
            static std::string DIRECTIVE_SPEECH_STARTED  = "SpeechStarted";
            static std::string DIRECTIVE_SPEECH_FINISHED = "SpeechFinished";
    }

    //SpeechRecognizer.Recognize
     namespace SpeechRecognizer
     {
            static std::string NAME_SPACE                = "SpeechRecognizer";
            static std::string HEADER_RECOGNIZER_STATE   = "RecognizerState";
            static std::string HEADER_RECOGNIZER_NAME    = "Recognize";
            static std::string HEADER_MESSAGE_ID         = "messageId";
            static std::string HEADER_DIALOG_REQUEST_ID  = "dialogRequestId";

            static std::string DIRECTIVE_STOP_CAPTURE    = "StopCapture";
            static std::string DIRECTIVE_EXPECT_SPEECH   = "ExpectSpeech";

            static std::string PAYLOAD_WAKEWORD_NAME     = "wakeword";
            static std::string PAYLOAD_WAKEWORD_ALEXA    = "ALEXA";
            static std::string PAYLOAD_PARAM_PROFILE     = "profile";
            static std::string PAYLOAD_PARAM_FORMAT      = "format";
            static std::string AUDIO_FORMAT              = "AUDIO_L16_RATE_16000_CHANNELS_1";

            static std::string PROFILE_CLOSE_TALK        = "CLOSE_TALK";
            static std::string PROFILE_NEAR_FIELD        = "NEAR_FIELD";
            static std::string PROFILE_FAR_FIELD         = "FAR_FIELD";
     }
}


#endif // JSON_H
