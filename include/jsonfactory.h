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
#ifndef JSONFACTORY_H
#define JSONFACTORY_H

#include "json.h"

namespace AVSJson
{
    struct AlertsState
    {
        std::string token;
        std::string type;
        std::string scheduledTime;
    };

    struct VolumeState
    {
        long volume;
        std::string muted; //boolean type true:false
    };

    struct SpeechState
    {
        std::string token;
        long offsetInMilliseconds;
        std::string playerActivity;
    };

    struct PlaybackState
    {
        std::string token;
        long offsetInMilliseconds;
        std::string playerActivity;
    };

    struct RecognizeState
    {
        std::string wakeword;
    };

    struct SynchronizeStateEvent
    {
        AlertsState    * alertsState;
        PlaybackState  * playbackState;
        VolumeState    * volumeState;
        SpeechState    * speechState;
        RecognizeState * recognizeState;

        std::string messageId;
    };

    struct PayloadPlaybackFailedParam
    {
        std::string token;

        std::string playbackState_token;
        std::string playbackState_offset;
        std::string playbackState_activity;

        std::string error_type;
        std::string error_message;
    };

    class JsonFactory
    {
        public:
            JsonFactory();
            ~JsonFactory(){}

            /*!
             * AVS System.SynchronizeState Event
             * \brief JsonFactory::createHTTPJsonSyncstateObj
             * \param event
             * \return
             *
             *  {
             *       "context": [
             *           {{Alerts.AlertsState}},
             *           {{AudioPlayer.PlaybackState}},
             *           {{Speaker.VolumeState}},
             *           {{SpeechSynthesizer.SpeechState}}
             *       ],
             *       "event": {
             *           "header": {
             *               "namespace": "System",
             *               "name": "SynchronizeState",
             *               "messageId": "{{STRING}}"
             *           },
             *           "payload": {
             *           }
             *       }
             *   }
             *
             */
            const std::string createSystemSynchronizeEvent( const SynchronizeStateEvent * event );

            /*!
             * AVS SpeechRecognizer.Recognize Event
             * \brief JsonFactory::createHTTPJsonAudioObj
             * \param event
             * \param dialog_request_id
             * \return
             *
             *  {
             *       "context": [
             *           {{Alerts.AlertsState}},
             *           {{AudioPlayer.PlaybackState}},
             *           {{Speaker.VolumeState}},
             *           {{SpeechSynthesizer.SpeechState}}
             *       ],
             *       "event": {
             *           "header": {
             *               "namespace": "SpeechRecognizer",
             *               "name": "Recognize",
             *               "messageId": "{{STRING}}",
             *               "dialogRequestId": "{{STRING}}"
             *           },
             *           "payload": {
             *               "profile": "{{STRING}}",
             *               "format": "{{STRING}}"
             *           }
             *       }
             *   }
             *
             *
             */
            const std::string  createSpeechRecognizerEvent( const SynchronizeStateEvent * event , const std::string & dialog_request_id );

            /*!
             * AVS SpeechStarted Event
             * \brief JsonFactory::createSpeechSynthesizerStartEvent
             * \param messageId
             * \param token
             * \return
             *
             *  {
             *      "event": {
             *          "header": {
             *              "namespace": "SpeechSynthesizer",
             *              "name": "SpeechStarted",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *      }
             *  }
             *
             */
            const std::string createSpeechSynthesizerStartEvent ( const std::string & messageId , const std::string & token );

            /*!
             * AVS SpeechFinished Event
             * \brief JsonFactory::createSpeechSynthesizerFinishEvent
             * \param messageId
             * \param token
             * \return
             *
             *  {
             *      "event": {
             *          "header": {
             *              "namespace": "SpeechSynthesizer",
             *              "name": "SpeechFinished",
             *               "messageId": "{{STRING}}"
             *           },
             *           "payload": {
             *               "token": "{{STRING}}"
             *           }
             *       }
             *    }
             *
             *
             */
            const std::string createSpeechSynthesizerFinishEvent( const std::string & messageId , const std::string & token );

            /*!
             * AVS SetAlertSucceeded Event
             * The SetAlertSucceeded event must be sent to AVS after receiving a SetAlert directive, when
             * the client successfully sets the alert
             * \brief createSetAlertSucceededEvent
             * \param messageId
             * \param token
             * \return
             *
             * {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "SetAlertSucceeded",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *       }
             *   }
             *
             */
            const std::string createSetAlertSucceededEvent( const std::string & messageId , const std::string & token );


            /*!
             * AVS SetAlertFailed Event
             * The SetAlertFailed event must be send to AVS after receiving a SetAlert directive, when the client fails to sets
             * an alert.
             * \brief createSetAlertFailedEvent
             * \param messageId
             * \param token
             * \return
             *
             *   {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "SetAlertFailed",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *      }
             *  }
             *
             */
            const std::string createSetAlertFailedEvent( const std::string & messageId , const std::string & token );


            /*!
             * AVS DeleteAlertFailed Event
             * The DeleteAlertFailed event must be sent to AVS after receiving a DeleteAlert directive, when the client failes
             * to delete or cancel an existing alert.
             * \brief createDeleteAlertFailedEvent
             * \param messageId
             * \param token
             * \return
             *
             *      {
             *          "event": {
             *              "header": {
             *                  "namespace": "Alerts",
             *                  "name": "DeleteAlertFailed",
             *                  "messageId": "{{STRING}}"
             *              },
             *              "payload": {
             *                  "token": "{{STRING}}"
             *              }
             *           }
             *       }
             *
             */
            const std::string createDeleteAlertFailedEvent( const std::string & messageId , const std::string & token );


            /*!
             * AVS AlertStarted Event
             * The AlertStarted event must be sent to AVS when an alert is triggered at its scheduled time
             * \brief createAlertStartedEvent
             * \param messageId
             * \param token
             * \return
             *
             *   {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "AlertStarted",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *    }
             *
             *
             */
            const std::string createAlertStartedEvent( const std::string & messageId , const std::string & token );


            /*!
             * AVS AlertStopped Event
             * https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/alerts
             * \brief createAlertStoppedEvent
             * \param messageId
             * \param token
             * \return
             *
             *   {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "AlertStopped",
             *              "messageId": "{STRING}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *      }
             *    }
             *
             */
            const std::string createAlertStoppedEvent( const std::string & messageId , const std::string & token );

            /*!
             * AVS AlertEnteredForeground Event
             * https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/alerts
             * \brief createAlertEnteredForegroundEvent
             * \param messageId
             * \param token
             * \return
             *
             * {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "AlertEnteredForeground",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *       }
             *  }
             *
             *
             */
            const std::string createAlertEnteredForegroundEvent( const std::string & messageId , const std::string & token );


            /*!
             * AVS AlertEnteredBackgroundEvent
             * https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/alerts
             * \brief createAlertEnteredBackgroundEvent
             * \param messageId
             * \param token
             * \return
             *
             *  {
             *      "event": {
             *          "header": {
             *              "namespace": "Alerts",
             *              "name": "AlertEnteredBackground",
             *              "messageId": "{{STRING}}"
             *          },
             *          "payload": {
             *              "token": "{{STRING}}"
             *          }
             *       }
             *  }
             *
             *
             */
            const std::string createAlertEnteredBackgroundEvent( const std::string & messageId , const std::string & token );


            /*!
             * \brief createPlaybackStartedEvent
             * \param messageId
             * \param token
             * \param offset
             * \return
             */
            const std::string createPlaybackStartedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackNearlyFinishedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createProgressReportDelayElapsedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createProgressReportIntervalElapsedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackStutterStartedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackStutterFinishedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackFinishedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackFailedEvent( const std::string & messageId , const PayloadPlaybackFailedParam * param );
            const std::string createPlaybackStoppedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackPausedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackResumedEvent( const std::string & messageId , const std::string & token , long offset );
            const std::string createPlaybackQueueClearedEvent( const std::string & messageId );

            const std::string createPlayCommandIssuedEvent( const SynchronizeStateEvent * event  , const std::string & messageId );
            const std::string createPauseCommandIssuedEvent( const SynchronizeStateEvent * event , const std::string & messageId );
            const std::string createNextCommandIssuedEvent( const SynchronizeStateEvent * event , const std::string & messageId);
            const std::string createPreviousCommandIssuedEvent( const SynchronizeStateEvent * event , const std::string & messageId );

            const std::string createVolumeChangedEvent( const std::string & messageId , long volume , bool muted );
            const std::string createMuteChangedEvent( const std::string & messageId , long volume , bool muted );

            /*!
             * \brief createSettingsUpdateEvent
             * \param messageId
             * \param key: locale
             * \param value: en-US, en-GB , de-DE
             * \return
             */
            const std::string createSettingsUpdateEvent( const std::string & messageId , const std::string & key , const std::string & value );

        protected:

            /*!
             * AVS Alerts.AlertsState
             * \brief JsonFactory::createAlertsContext
             * \param state
             * \return
             *
             *  {
             *      "header": {
             *          "namespace": "Alerts",
             *          "name": "AlertsState"
             *      },
             *      "payload": {
             *          "allAlerts": [
             *                        {
             *                  "token": "{{STRING}}",
             *                  "type": "{{STRING}}",
             *                  "scheduledTime": "{{STRING}}"
             *              }
             *          ],
             *          "activeAlerts": [
             *                        {
             *                  "token": "{{STRING}}",
             *                  "type": "{{STRING}}",
             *                  "scheduledTime": "{{STRING}}"
             *              }
             *          ]
             *      }
             *  }
             *
             *
             */
            const std::string createAlertsContext( const AlertsState * state );

            /*!
             * AVS AudioPlayer.PlaybackState
             * \brief JsonFactory::createPlaybackContext
             * \param state
             * \return
             *
             *  {
             *       "header": {
             *           "namespace": "AudioPlayer",
             *           "name": "PlaybackState"
             *       },
             *       "payload": {
             *           "token": "{{STRING}}",
             *           "offsetInMilliseconds": {{LONG}},
             *           "playerActivity": "{{STRING}}"
             *       }
             *   }
             *
             *
             */
            const std::string createPlaybackContext( const PlaybackState * state );

            /*!
             * AVS Speaker.VolumeState
             * \brief JsonFactory::createSpeakerContext
             * \param state
             * \return
             *
             *  {
             *       "header": {
             *           "namespace": "Speaker",
             *           "name": "VolumeState"
             *       },
             *       "payload": {
             *           "volume": {{LONG}},
             *           "muted": {{BOOLEAN}}
             *       }
             *   }
             *
             */
            const std::string createSpeakerContext( const VolumeState * state );

            /*!
             * AVS SpeechSynthesizer.SpeechState
             * \brief JsonFactory::createSpeechSynthesizerContext
             * \param state
             * \return
             *
             *  {
             *       "header": {
             *           "namespace": "SpeechSynthesizer",
             *           "name": "SpeechState"
             *       },
             *       "payload": {
             *           "token": "{{STRING}}",
             *           "offsetInMilliseconds": {{LONG}},
             *           "playerActivity": "{{STRING}}"
             *       }
             *   }
             *
             *
             */
            const std::string createSpeechSynthesizerContext( const SpeechState * state );
            const std::string createSpeechRecognizerContext( const RecognizeState * state );

            const std::string createAlertCustomEvent( const std::string & event , const std::string & messageId , const std::string & token );
            const std::string createPlaybackCustomEvent( const std::string & event , const std::string & messageId , const std::string & token , long offset );
            const std::string createCommandCustomEvent( const SynchronizeStateEvent * event  , const std::string & event_name , const std::string & messageId );
            const std::string createSpeakerCustomEvent( const std::string & event , const std::string & messageId , long volume , bool muted );
    };

}

#endif // JSONFACTORY_H

