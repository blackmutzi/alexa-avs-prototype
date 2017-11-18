TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -Wdeprecated-declarations

# ALSA = -lasound , CURL = -lcurl
LIBS += -lmp3lame -lboost_system -lboost_thread -lasound -lcurl -lnghttp2 -lssl -lcrypto -levent -levent_openssl -levent_core

INCLUDEPATH += $$PWD/include/

SOURCES += main.cpp \
    avsapplication.cpp \
    avscontroller.cpp \
    wakewordipc.cpp \
    wakewordipcfactory.cpp \
    wakewordipcsocket.cpp \
    serversocket.cpp \
    wakewordipcconnectedclient.cpp \
    logger.cpp \
    audiocapture.cpp \
    httprequest.cpp \
    avsclient.cpp \
    httpcontentfactory.cpp \
    httpheaderfactory.cpp \
    httpcontent.cpp \
    httpheader.cpp \
    http2header.cpp \
    httpcurlhandler.cpp \
    httpresponse.cpp \
    jsonfactory.cpp \
    httpclientdirectiveevent.cpp \
    jsonreader.cpp \
    audioplayback.cpp \
    nghttp2handler.cpp \
    nghttp2api.cpp \
    nghttp2responsemanager.cpp \
    fileloader.cpp \
    jsonfileloader.cpp \
    http2multipartanalyse.cpp \
    http2client.cpp \
    nghttp2multiplexhandler.cpp \
    avsrequestbuilder.cpp \
    http2clienteventmanager.cpp \
    avsauthenticator.cpp \
    avsconfigauthenticator.cpp \
    bytefileloader.cpp \
    avslame.cpp \
    avsplayback.cpp \
    avsplaybackcontroller.cpp

HEADERS += \
    include/stdafx.h \
    include/avsapplication.h \
    include/avscontroller.h \
    include/wakeword.h \
    include/wakeworddetectedhandler.h \
    include/wakewordreadyinterface.h \
    include/wakewordipc.h \
    include/wakewordipcfactory.h \
    include/wakewordipcsocket.h \
    include/thread.h \
    include/Runnable.h \
    include/network.h \
    include/serversocket.h \
    include/wakewordipcconnectedclient.h \
    include/logger.h \
    include/audio.h \
    include/audiocapture.h \
    include/httprequest.h \
    include/avsclient.h \
    include/httpcontentfactory.h \
    include/httpheaderfactory.h \
    include/httpcontent.h \
    include/httpheader.h \
    include/http2header.h \
    include/httpcurlhandler.h \
    include/httpresponse.h \
    include/jsonfactory.h \
    include/json.h \
    include/httpclientdirectiveevent.h \
    include/nlohmann_json.h \
    include/jsonreaderdirective.h \
    include/audioplayback.h \
    include/nghttp2handler.h \
    include/nghttp2api.h \
    include/nghttp2responsemanager.h \
    include/fileloader.h \
    include/jsonfileloader.h \
    include/http2multipartanalyse.h \
    include/http2client.h \
    include/nghttp2multiplexhandler.h \
    include/avsrequestbuilder.h \
    include/http2clienteventmanager.h \
    include/avsauthenticator.h \
    include/avsconfigauthenticator.h \
    include/bytefileloader.h \
    include/avslame.h \
    include/avsplayback.h \
    include/directivenlistener.h \
    include/avsclientlistener.h \
    include/avsplaybackcontroller.h

