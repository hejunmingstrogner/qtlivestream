#-------------------------------------------------
#
# Project created by QtCreator 2018-10-16T14:42:20
#
#-------------------------------------------------

QT       += core gui
QT       += multimediawidgets
QT       += xml
QT       += multimedia
QT       += network
QT       += widgets
QT       += serialport
QT       += opengl
QT       += network
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT +=  opengl openglwidgets


TARGET = LiveStream
TEMPLATE = app

#CONFIG += c++17 utf8_source
CONFIG   += c++11
CONFIG -= app_bundle
TEMPLATE = app

SDKPATHNAME=libs
SDKLIBPATHNAME=x86
SDKDLLPATHNAME=x86

!contains(QMAKE_TARGET.arch, x86_64) {
  SDKLIBPATHNAME=x86
  SDKDLLPATHNAME=x86
} else {
  SDKLIBPATHNAME=x86_64
  SDKDLLPATHNAME=x86_64
}

MDK_SDK = $$PWD/../../mdk-sdk
INCLUDEPATH += $$MDK_SDK/include/mdk
contains(QT_ARCH, x.*64) {
  android: MDK_ARCH = x86_64
  else:linux: MDK_ARCH = amd64
  else: MDK_ARCH = x64
} else:contains(QT_ARCH, .*86) {
  MDK_ARCH = x86
} else:contains(QT_ARCH, a.*64.*) { # arm64-v8a
  android: MDK_ARCH = arm64-v8a
  else: MDK_ARCH = arm64
} else:contains(QT_ARCH, arm.*) {
  android: MDK_ARCH = armeabi-v7a
  else:linux: MDK_ARCH = armhf
  else: MDK_ARCH = arm
}

macx {
  LIBS += -F$$MDK_SDK/lib -F/usr/local/lib -framework mdk
} else {
  LIBS += -L$$MDK_SDK/lib/$$MDK_ARCH -lmdk
  win32: LIBS += -L$$MDK_SDK/bin/$$MDK_ARCH # qtcreator will prepend $$LIBS to PATH to run targets
}
linux: LIBS += -Wl,-rpath-link,$$MDK_SDK/lib/$$MDK_ARCH # for libc++ symbols
linux: LIBS += -Wl,-rpath,$$MDK_SDK/lib/$$MDK_ARCH

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        camera.cpp \
        mainwindow.cpp \
        openglwidget.cpp \
        pcmcaptureandplay.cpp \
        pullstream.cpp \
        QMDKWindow.cpp \
        QMDKWidget.cpp \
        rtmp.cpp \
        agoraconfig.cpp \
        agoraobject.cpp \
        agoraqtjson.cpp

HEADERS  += QMDKWidget.h \
      QMDKWindow.h\
      camera.h \
      mainwindow.h \
      openglwidget.h \
      pcmcaptureandplay.h \
      pullstream.h  \
      global.h \
      MediaInfo.h \
      Player.h \
      RenderAPI.h \
      VideoFrame.h \
      rtmp.h \
      stdafx.h \
      agoraconfig.h \
      agoraobject.h \
      agoraqtjson.h


FORMS    +=\
            camera.ui \
            mainwindow.ui \
            pullstream.ui \
            rtmp.ui


mac {
  RPATHDIR *= @executable_path/Frameworks $$MDK_SDK/lib
  QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
  isEmpty(QMAKE_LFLAGS_RPATH): QMAKE_LFLAGS_RPATH=-Wl,-rpath,
  for(R,RPATHDIR) {
    QMAKE_LFLAGS *= \'$${QMAKE_LFLAGS_RPATH}$$R\'
  }
}


exists( $$PWD/$${SDKPATHNAME}) {
  AGORASDKPATH = $$PWD/$${SDKPATHNAME}
  AGORASDKDLLPATH = .\\$${SDKPATHNAME}\\$${SDKDLLPATHNAME}
} else {
  AGORASDKPATH = $$PWD/../../$${SDKPATHNAME}
  AGORASDKDLLPATH =..\\..\\$${SDKPATHNAME}\\$${SDKDLLPATHNAME}
}

win32: {
INCLUDEPATH += $${AGORASDKPATH}/include
LIBS += -L$${AGORASDKPATH}/$${SDKLIBPATHNAME} -lagora_rtc_sdk
LIBS += User32.LIB

CONFIG(debug, debug|release) {
  QMAKE_POST_LINK +=  copy $${AGORASDKDLLPATH}\*.dll .\Debug
} else {
  QMAKE_POST_LINK +=  copy $${AGORASDKDLLPATH}\*.dll .\Release
  QMAKE_POST_LINK += && windeployqt Release\OpenLive.exe
}

}

#QMAKE_CXXFLAGS += -std:c++20 -Zc:__cplusplus
#mp-sdk
#LIBS += -L$$PWD/../../LiveStream/mdk-sdk/lib/x64/

#INCLUDEPATH += $$PWD/../../LiveStream/mdk-sdk/include/mdk
#DEPENDPATH += $$PWD/../../LiveStream/mdk-sdk/lib/x64

#输入日志生成release需加
DEFINES += QT_MESSAGELOGCONTEXT
