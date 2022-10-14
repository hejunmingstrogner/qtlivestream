#include "rtmp.h"
#include "ui_rtmp.h"

rtmp::rtmp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rtmp)
{
    ui->setupUi(this);
    initVideoLayer();
}

void rtmp::initVideoLayer()
{
    auto videolayer = ui->videolayer;
    videolayer->setMedia("rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp");
    videolayer->play();
}

rtmp::~rtmp()
{
    delete ui;
}
