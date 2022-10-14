#ifndef PULLSTREAM_H
#define PULLSTREAM_H

#include <Memory>
#include <QVariant>
#include <QMainWindow>
#include <mutex>
#include <qtimer.h>

#include "./libs/include/IAgoraRtcEngine.h"
using namespace agora::rtc;

#include <QDialog>

namespace Ui {
class pullstream;
}

typedef struct eTagVideoStats {
    eTagVideoStats(){;}
    eTagVideoStats(unsigned int ui,int nlastmiledelay = 0,int nwidth = 0,int nheight = 0,int nfps = 0,int nbitrate = 0,QWidget *pwidget = Q_NULLPTR,int nindex = 0):
        uid(ui),nLastmileDelay(nlastmiledelay),nWidth(nwidth),nHeight(nheight),nFps(nfps),nBitrate(nbitrate),pWidget(pwidget),nIndex(nindex){;}
    eTagVideoStats(const eTagVideoStats& vss) {
        *this = vss;
    }
    eTagVideoStats& operator=(const eTagVideoStats& vss) {
        if(this != &vss) {
            uid = vss.uid;
            nLastmileDelay = vss.nLastmileDelay;
            nWidth = vss.nWidth;
            nHeight = vss.nHeight;
            nFps = vss.nFps;
            nBitrate = vss.nBitrate;
            pWidget = vss.pWidget;
            nIndex = vss.nIndex;
        }

        return *this;
    }

    unsigned int uid;
    int nLastmileDelay;
    int nWidth;
    int nHeight;
    int nFps;
    int nBitrate;
    QWidget* pWidget;
    int nIndex;
}Video_Stats,*PVideo_Stats,*LPVideoStats;
typedef QMap<unsigned int,Video_Stats> qIVS;

class pullstream : public QDialog
{
    Q_OBJECT

public:
    explicit pullstream(QWidget *parent = nullptr);
    ~pullstream();


public slots:
    void on_pushButton_clicked();
    void joinchannel(QMainWindow* pMainWnd,const QString& qsChannel,uint uid);
    void leavechannel();

    void receive_videoStopped();
    void receive_joinedChannelSuccess(const QString &qsChannel, unsigned int uid, int elapsed);
    void receive_userJoined(unsigned int uid, int elapsed);
    void receive_userOffline(unsigned int uid, USER_OFFLINE_REASON_TYPE reason);
    void receive_firstLocalVideoFrame(int width, int height, int elapsed);
    void receive_firstRemoteVideoDecoded(unsigned int uid, int width, int height, int elapsed);
    void receive_firstRemoteVideoFrameDrawn(unsigned int uid, int width, int height, int elapsed);
    void receive_localVideoStats(const LocalVideoStats &stats);
    void receive_remoteVideoStats(const RemoteVideoStats &stats);
    void receive_rtcStats(const RtcStats &stats);

    void receive_timer_pfs();

    bool eventFilter(QObject *watched,QEvent *event);

private slots:

    void on_camera_clicked(bool checked);

    void on_mic_clicked(bool checked);

private:
    Ui::pullstream *ui;
    std::mutex  m_vsMutex;
    qIVS    m_qivs;
    uid_t   m_uid;
    int     m_nlastmileDelay;
    QTimer  *m_timer_fps;
    uid_t   m_bigUid;
    QString m_qsChannel;

    void adjustPos();

};

#endif // PULLSTREAM_H
