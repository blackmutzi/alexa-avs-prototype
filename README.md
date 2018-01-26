[![N|Solid](https://github.com/alexa/alexa-avs-sample-app/wiki/assets/alexa-logo-3.png)](https://developer.amazon.com/de/alexa-voice-service)
# alexa-avs-prototype (c++) [![travis](https://travis-ci.org/blackmutzi/alexa-avs-prototype.svg?branch=master)](https://travis-ci.org/)
Alexa Voice Service (AVS) client in c++ for raspberry pi or any Linux Distribution.
Project tested on Gentoo(x86_64) and Raspbian(arm).

The Prototype is deprecated use alexa-sdk-framewok/alexa-amazon-client, is already released and has more improvment.

# what you need?
* A Linux Distribution with PulseAudio Support.
* Amazon Developer Account.
* Follow the steps [here](https://github.com/alexa/alexa-avs-sample-app/wiki/Raspberry-Pi) to create a security profile.
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

{
 "client_id":" YOUR_CLIENT_ID_HERE ",
 "client_secret":" YOUR_CLIENT_SECRET_ID_HERE ",
 "code_grant":" YOUR_CODE_GRANT_HERE",
 "product_id":" YOUR_PRODUCT_NAME_HERE ",
 "redirect_uri":"https://localhost:3000/authresponse",
 "refresh_token":"AVS-CLIENT_SET_TOKEN_AUTOMATIC"
}

```
and write client_id, client_secret and product_id into the config. ( @see amazon-security-profile )
#### second Step
start now prototype ...
```sh
./alexa-avs-prototype
```
the avs-client show you a link, copy it in your Browser. Login Now ..

and get the grant code, save the grant code in the ./alexa-avs-prototype/src/config/config.json file, finish.

# depends installing ( debian )
```sh
install:
  - sudo apt-get install libcurl4-gnutls-dev
  - sudo apt-get install libboost-all-dev
  - sudo apt-get install gcc-4.9
  - sudo apt-get install g++-4.9
  - sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 100
  - sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 100
  - sudo add-apt-repository -y "deb http://de.archive.ubuntu.com/ubuntu artful main restricted universe multiverse"
  - sudo apt-get update -qq
  - sudo apt-get install libnghttp2-dev
  - sudo apt-get install libevent-dev
  - sudo apt-get install libmp3lame-dev
  - sudo apt-get install libssl-dev
  - sudo apt-get install libasound2-dev
 ```
