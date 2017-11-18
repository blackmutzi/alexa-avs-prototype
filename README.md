[![N|Solid](https://github.com/alexa/alexa-avs-sample-app/wiki/assets/alexa-logo-3.png)](https://developer.amazon.com/de/alexa-voice-service)
# alexa-avs-prototype (c++) [![travis](https://travis-ci.org/blackmutzi/alexa-avs-prototype.svg?branch=master)](https://travis-ci.org/)
Alexa Voice Service (AVS) client in c++ for raspberry pi or any Linux Distribution.
Project tested on Gentoo(x86_64) and Raspbian(arm).

# what you need?
* A Linux Distribution with PulseAudio Support.
* Amazon Developer Account.
* Follow the steps [here](https://github.com/alexa/alexa-avs-sample-app/wiki/Raspberry-Pi) to create a security profile.
* From Security Profile, you need ProductID, ClientID, and ClientSecret.
* And a WakeWordEngine Client from [here]( https://github.com/alexa/alexa-avs-sample-app/tree/master/samples/wakeWordAgent ).

# download && build
```sh
git clone https://github.com/blackmutzi/alexa-avs-prototype
cd ./alexa-avs-prototype
qmake ./src/alexa-avs-prototype.pro
make
```
# avs-configuration
#### first Step
go to config directory and modify ./src/config/config.json file 
```sh
cd ./alexa-avs-prototype/src/config/
nano config.json 
```
and write client_id, client_secret and product_id into the config. ( @see amazon-security-profile )
#### second Step
start now prototype ...
```sh
./alexa-avs-prototype
```
the avs-client show you a link, copy it in your Browser. Login Now ..

and get the grant code, save the grant code in the ./alexa-avs-prototype/src/config/config.json file, finish.

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
# solution for libssl-dev and libnghttp2-dev
add two [repositorys](https://github.com/superjamie/lazyweb/wiki/Raspberry-Pi-Debian-Backports) stretch and jessie-backports into /etc/apt/sources.list file.
get libssl-dev and libnghttp2-dev
```sh
apt-get -t jessie-backports install libssl-dev
apt-get -t stretch install libnghttp2-dev
```
after remove repositorys and run apt-get update. FINISH.   
