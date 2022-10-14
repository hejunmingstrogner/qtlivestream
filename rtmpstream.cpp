#include "rtmpstream.h"
#include "ui_rtmpstream.h"

rtmpstream::rtmpstream(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rtmpstream)
{
    ui->setupUi(this);
 //   initGLvideo();
}
/*
void rtmpstream::initGLvideo()
{
   auto player = ui->openGLWidget;
   player->setMedia("rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp");
   player->play();
}
*/
rtmpstream::~rtmpstream()
{
    delete ui;
}
