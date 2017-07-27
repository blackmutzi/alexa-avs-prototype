[![N|Solid](https://github.com/alexa/alexa-avs-sample-app/wiki/assets/alexa-logo-3.png)](https://developer.amazon.com/de/alexa-voice-service)
# alexa-avs-prototype ( C++ )
Alexa Voice Service (AVS) client in C++ for raspberry pi or any Linux Distribution -  This is a work in progress.
Project is tested with Linux Gentoo(x86_64) and Raspbian(ARM).
# What is AVS?
Alexa Voice Service (AVS) is Amazon’s intelligent voice recognition and natural language understanding service that allows you as a developer to voice-enable any connected device that has a microphone and speaker.
# What you need?
* A Linux Distribution with PulseAudio Support.
* Amazon Developer Account.
* Follow the steps [here](https://github.com/alexa/alexa-avs-sample-app/wiki/Raspberry-Pi) to create a security profile.
* From Security Profile, you need ProductID, ClientID, and ClientSecret.
* And a WakeWordEngine Client from [here]( https://github.com/alexa/alexa-avs-sample-app/tree/master/samples/wakeWordAgent ).

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
# DEPENDS ( debian / raspbian packages )
* libevent-dev ( version 2.0.5 )
* libmp3lame-dev ( version 3.99.5 )
* libcurl-dev ( version 7.54.0 )
* libssl-dev ( version 1.0.2 - ALPN h2 Protocol needed )
* libboost-all-dev ( tested version with 1.55 , 1.63 )
* libasound2-dev ( version 1.0.28 )
* libnghttp2-dev ( version 1.18.1-1 , 1.22.0 , 1.24.0 or higher )   
# Known Bugs:
* libboost-all-dev version 1.62 - compile error 
* libnghttp2-dev version 0.6.4.2 - to much nghttp2 Bugs ( required 1.22.0 or higher ) 
* libssl-dev version 1.0.1 - ALPN protocol: h2 is not negotiated error message ( required version 1.0.2 )
# Solution for libssl-dev and libnghttp2-dev
* add [jessie-backports](https://github.com/superjamie/lazyweb/wiki/Raspberry-Pi-Debian-Backports) repository
* add stretch repository 
* deb http://httpredir.debian.org/debian jessie-backports main contrib non-free
* deb http://httpredir.debian.org/debian stretch main contrib non-free
```sh
apt-get -t jessie-backports install libssl-dev
apt-get -t stretch install libnghttp2-dev
```
# Alexa Voice Service Features ( This is work in progress )
* ALERT ( [AlertInterface](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/alerts) )
* Audioplayer ( [Audioplayer](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/audioplayer) )
* Notification ( [NotificationInterface](https://developer.amazon.com/public/solutions/alexa/alexa-voice-service/reference/notifications) ) 
