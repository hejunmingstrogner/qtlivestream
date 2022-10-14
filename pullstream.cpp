#include "pullstream.h"
#include "ui_pullstream.h"
#include "mainwindow.h"
#include "stdafx.h"
#include "agoraobject.h"

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

pullstream::pullstream(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pullstream)
{
    ui->setupUi(this);

    CAgoraObject* pObject = CAgoraObject::getInstance();
    connect(pObject,SIGNAL(sender_videoStopped()),
            this,SLOT(receive_videoStopped()));
    connect(pObject,SIGNAL(sender_joinedChannelSuccess(const QString&,unsigned int ,int)),
            this,SLOT(receive_joinedChannelSuccess(const QString&,unsigned int,int)));
    connect(pObject,SIGNAL(sender_userJoined(unsigned int,int)),
            this,SLOT(receive_userJoined(unsigned int,int)));
    connect(pObject,SIGNAL(sender_userOffline(unsigned int,USER_OFFLINE_REASON_TYPE)),
            this,SLOT(receive_userOffline(unsigned int,USER_OFFLINE_REASON_TYPE)));
    connect(pObject,SIGNAL(sender_firstLocalVideoFrame(int,int,int)),
            this,SLOT(receive_firstLocalVideoFrame(int,int,int)));
    connect(pObject,SIGNAL(sender_firstRemoteVideoDecoded(unsigned int,int,int,int)),
            this,SLOT(receive_firstRemoteVideoDecoded(unsigned int,int,int,int)));
    connect(pObject,SIGNAL(sender_firstRemoteVideoFrameDrawn(unsigned int,int,int,int)),
            this,SLOT(receive_firstRemoteVideoFrameDrawn(unsigned int,int,int,int)));
    connect(pObject,SIGNAL(sender_localVideoStats(LocalVideoStats)),
            this,SLOT(receive_localVideoStats(LocalVideoStats)));
    connect(pObject,SIGNAL(sender_remoteVideoStats(RemoteVideoStats)),
            this,SLOT(receive_remoteVideoStats(RemoteVideoStats)));
    connect(pObject,SIGNAL(sender_rtcStats(RtcStats)),
            this,SLOT(receive_rtcStats(RtcStats)));

    connect(this,SIGNAL(clicked()),this,SLOT(mouseClicked()));

}

pullstream::~pullstream()
{
    delete ui;
}

void pullstream::on_pushButton_clicked()
{

     MainWindow * maindown = new MainWindow();
     this->close();
     maindown->show();

}


void pullstream::joinchannel(QMainWindow* pMainWnd,const QString& qsChannel,uint uid)
{
    qDebug(__FUNCTION__);
    this->show();

    CAgoraObject::getInstance()->SetDefaultParameters();

    QString token = CAgoraObject::getInstance()->GetAppToken();
    CAgoraObject::getInstance()->joinChannel(token,qsChannel,uid);

}


void pullstream::leavechannel()
{
    qDebug(__FUNCTION__);

    m_timer_fps->stop();
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        m_qivs.clear();
        delete m_timer_fps;
        m_bigUid = 0;
        m_uid = 0;
    }

    CAgoraObject::getInstance()->leaveChannel();
    this->hide();
}

void pullstream::receive_videoStopped()
{
    qDebug(__FUNCTION__);
}

void pullstream::receive_joinedChannelSuccess(const QString &qsChannel, unsigned int uid, int elapsed)
{
    qDebug("%s %s %u",__FUNCTION__,qsChannel.toStdString().c_str(),uid);

    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        m_uid = uid;
        if(!m_qivs.contains(uid)) {
            m_qivs[uid] = Video_Stats(uid);
        }
    }

    adjustPos();

   /* QString qsChannelInfo;
    m_qsChannel = qsChannel;
    qsChannelInfo.asprintf("%s %u",qsChannel.toStdString().c_str(),uid);*/
  //  m_uper->setChannelName(qsChannelInfo);
    m_bigUid = uid;
    m_timer_fps->start(2000);
}

void pullstream::receive_userJoined(uid_t uid, int elapsed)
{
    qDebug("%s %u",__FUNCTION__,uid);
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(!m_qivs.contains(uid)) {
            m_qivs[uid] = Video_Stats(uid);
        }
    }

    adjustPos();
}

void pullstream::receive_userOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
    qDebug("%s %u",__FUNCTION__,uid);
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(m_qivs.contains(uid)) {
            auto it = m_qivs.find(uid);
            m_qivs.erase(it);
        }
    }

    adjustPos();
}

void pullstream::receive_firstLocalVideoFrame(int width, int height, int elapsed)
{
    qDebug(__FUNCTION__);
}

void pullstream::receive_firstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{
    qDebug(__FUNCTION__);
}

void pullstream::receive_firstRemoteVideoFrameDrawn(uid_t uid, int width, int height, int elapsed)
{
    qDebug(__FUNCTION__);
}

void pullstream::receive_localVideoStats(const LocalVideoStats &stats)
{
   // qDebug(__FUNCTION__);
    {
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        if(m_qivs.contains(m_uid)) {
            m_qivs[m_uid].nWidth = stats.encodedFrameWidth;
            m_qivs[m_uid].nHeight = stats.encodedFrameHeight;
            m_qivs[m_uid].nFps = stats.sentFrameRate;
            m_qivs[m_uid].nBitrate = stats.sentBitrate;
        }
    }
}

void pullstream::receive_remoteVideoStats(const RemoteVideoStats &stats)
{
  // qDebug(__FUNCTION__);
   {
       std::lock_guard<std::mutex> autoLock(m_vsMutex);
       if(m_qivs.contains(stats.uid)) {
           m_qivs[stats.uid].nLastmileDelay = stats.delay;
           m_qivs[stats.uid].nWidth = stats.width;
           m_qivs[stats.uid].nHeight = stats.height;
           m_qivs[stats.uid].nFps = stats.rendererOutputFrameRate;
           m_qivs[stats.uid].nBitrate = stats.receivedBitrate;
       }
   }
}

void pullstream::receive_rtcStats(const RtcStats &stats)
{
    m_nlastmileDelay = stats.lastmileDelay;

    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    if(m_qivs.contains(m_uid))
        m_qivs[m_uid].nLastmileDelay = stats.lastmileDelay;
}

void pullstream::adjustPos()
{
    ui->widget_r1_1->setVisible(true);
    ui->widget_r1_2->setVisible(true);
    ui->widget_r1_3->setVisible(true);
    ui->widget_r1_4->setVisible(true);
    ui->widget_r1_5->setVisible(true);
    ui->widget_r1_6->setVisible(true);

    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    auto it = m_qivs.begin();
    int nIndex = 0;
    while(m_qivs.end() != it) {
        if(it->uid == m_uid) {
            it->pWidget = ui->widget_r1_1;
            it->nIndex = 0;
            CAgoraObject::getInstance()->LocalVideoPreview((HWND)(it->pWidget->winId()),
                                                           TRUE,
                                                          (it->uid == m_bigUid) ? RENDER_MODE_FIT : RENDER_MODE_HIDDEN);
        } else {
                nIndex++;
            if(nIndex == 1) {
                it->pWidget = ui->widget_r1_2;
                it->nIndex = 1;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
            }

            if(nIndex == 2) {
                it->pWidget = ui->widget_r1_3;
                it->nIndex = 2;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
            }

            if(nIndex == 3) {
                it->pWidget = ui->widget_r1_4;
                it->nIndex = 3;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
            }
            if(nIndex == 4) {
                it->pWidget = ui->widget_r1_5;
                it->nIndex = 3;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
            }
            if(nIndex == 5) {
                it->pWidget = ui->widget_r1_6;
                it->nIndex = 3;
                CAgoraObject::getInstance()->RemoteVideoRender(it->uid,
                                                               (HWND)(it->pWidget->winId()),
                                                                (it->uid == m_bigUid) ? RENDER_MODE_FIT:RENDER_MODE_HIDDEN);
            }
        }
        it++;
    }

  //  m_uper->setAllRAtt(nIndex);
}


void pullstream::receive_timer_pfs()
{
    std::lock_guard<std::mutex> autoLock(m_vsMutex);
    if(m_qivs.contains(m_bigUid)) {
        QString qsFps;
        auto it = m_qivs.value(m_bigUid);
        qsFps.asprintf("SD-RTN: %dms.  Video: %dfps %dx%d",it.nLastmileDelay,it.nFps,it.nWidth,it.nHeight);
       // m_uper->setParam(qsFps);
    }
}

void pullstream::on_camera_clicked(bool checked)
{
    if(checked == Qt::Unchecked)
        CAgoraObject::getInstance()->MuteLocalVideo(FALSE);
    else if(checked == Qt::Checked)
        CAgoraObject::getInstance()->MuteLocalVideo(TRUE);
}


void pullstream::on_mic_clicked(bool checked)
{
    if(checked == Qt::Unchecked)
        CAgoraObject::getInstance()->MuteLocalAudio(FALSE);
    else if(checked == Qt::Checked)
        CAgoraObject::getInstance()->MuteLocalAudio(TRUE);
}

bool pullstream::eventFilter(QObject *watched,QEvent *event)
{
    /*
    bool bFind = false;
    int nIndex = 0;
    Video_Stats vsr,vsbig;
    if(watched==ui->widget_r1 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r1");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR1();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
            bFind = ( vsr.uid == m_bigUid ) ? false : true;
            nIndex = vsr.nIndex;
        }
    }
    else if(watched == ui->widget_r2 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r2");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR2();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
            bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }
    else if(watched == ui->widget_r3 && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick r3");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uper->getR3();
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
            bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }

    else if(watched == ui->widget_l && event->type() == QEvent::MouseButtonDblClick) {
        qDebug("DBClick local");
        std::lock_guard<std::mutex> autoLock(m_vsMutex);
        uid_t uid = m_uid;
        if(m_qivs.contains(uid) && m_qivs.contains(m_bigUid)) {
            vsr = m_qivs[uid];
            vsbig = m_qivs[m_bigUid];
            bFind = (vsr.uid == m_bigUid) ? false : true;
            nIndex = vsr.nIndex;
        }
    }

    if(bFind) {
        std::lock_guard<std::mutex> autolock(m_vsMutex);
        QRect qrr,qrbig;
        if(vsr.pWidget)
            qrr = vsr.pWidget->geometry();
        if(vsbig.pWidget)
            qrbig = vsbig.pWidget->geometry();

        vsr.pWidget->setGeometry(qrbig);
        vsbig.pWidget->setGeometry(qrr);
        vsr.pWidget->lower();
        vsbig.pWidget->raise();
        m_qivs[vsr.uid].nIndex = 0;
        m_qivs[vsbig.uid].nIndex = nIndex;

        QString qsSrc,qsDest;
        qsSrc.asprintf("%s %u",m_qsChannel.toStdString().c_str(),vsr.uid);
        qsDest.asprintf("  uid:%u",vsbig.uid);
        m_uper->switchUidText(qsSrc,qsDest,nIndex);

        m_bigUid = vsr.uid;
    }
*/
    return QWidget::eventFilter(watched,event);
}
