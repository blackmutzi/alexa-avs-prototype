[![N|Solid](https://github.com/alexa/alexa-avs-sample-app/wiki/assets/alexa-logo-3.png)](https://developer.amazon.com/de/alexa-voice-service)
# alexa-avs-prototype ( C++ )
Alexa Voice Service (AVS) client in C++ for RaspberryPI or any Linux Distribution -  This is a work in progress.
# What is AVS?
Alexa Voice Service (AVS) is Amazon’s intelligent voice recognition and natural language understanding service that allows you as a developer to voice-enable any connected device that has a microphone and speaker.
# What you need?
A Linux Distribution with PulseAudio Support. Project is tested with Gentoo(x86_64) and Raspbian(ARMv7).
Amazon Developer Account.
Follow the steps [here](https://github.com/alexa/alexa-avs-sample-app/wiki/Raspberry-Pi) to create a security profile.
From Security Profile, you need ProductID, ClientID, and ClientSecret.
And a WakeWordEngine Client from [here]( https://github.com/alexa/alexa-avs-sample-app/tree/master/samples/wakeWordAgent ).

# Download it
```sh
git clone https://github.com/blackmutzi/alexa-avs-prototype
cd ./alexa-avs-prototype
```
# Build it
```sh
qmake AVS-Prototype.pro
make -j2
```
# Project DEPENDS ( debian packages )
* libevent-dev
* libmp3lame-dev
* libcurl-dev ( version 7.54.0 or higher )
* libssl-dev ( version 1.0.2 - ALPN h2 Protocol needed ) - TLSv1.2
* libboost-all-dev ( version 1.63 required )
* libasound2-dev 
* libnghttp2 ( version 1.22.0 or higher ) 
# AVS Configuration
#### First Step
go to config directory and modify config.json file 
```sh
cd ./alexa-avs-prototype/config/
nano config.json 
```
and set only client_id, client_secret and product_id.
#### Second Step
run the AVS Prototype Client ...
```sh
chmod +x ./AVS-Prototype
./AVS-Prototype
```
the AVS-Client show you a link, copy it.
And login into the Amazon Security Profile Website. Then copy den code_grant key and save it into the config.json.
And Restart again AVS-Prototype. FINISH Configuration. AVS-Prototpye Client is now permanent READY.
# Alexa Voice Service Features ( This is work in progress )
* ALERT ( [AlertInterface](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/alerts) )
* Audioplayer ( [Audioplayer](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/audioplayer) )
* Notification ( [NotificationInterface](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/notifications) ) 
