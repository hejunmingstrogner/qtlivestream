#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>


#include "stdafx.h"
//#include <IAgoraRtcEngine.h>
#include "./libs/include/IAgoraRtcEngine.h"
using namespace agora::rtc;
Q_DECLARE_METATYPE(uid_t)
Q_DECLARE_METATYPE(USER_OFFLINE_REASON_TYPE)
Q_DECLARE_METATYPE(LocalVideoStats)
Q_DECLARE_METATYPE(RemoteVideoStats)
Q_DECLARE_METATYPE(RtcStats)

CAgoraConfig gAgoraConfig;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LiveStream_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    qRegisterMetaType<uid_t>();
    qRegisterMetaType<USER_OFFLINE_REASON_TYPE>();
    qRegisterMetaType<LocalVideoStats>();
    qRegisterMetaType<RemoteVideoStats>();
    qRegisterMetaType<RtcStats>();
    MainWindow w;
    w.show();
    return a.exec();
}
