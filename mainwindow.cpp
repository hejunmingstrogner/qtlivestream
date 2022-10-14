#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdafx.h"
#include <iostream>
#include <QDebug>
#include "pullstream.h"
#include "camera.h"
#include "agoraobject.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
//   glWidget = new OpenGLWidget(this);//创建OpenGL窗口，设于this（主窗口）内。
 //     glWidget->resize(800,570);
 //     glWidget->move(0,30);
  //    glWidget->show();
//   initGLvideo();
   initWindow();
}

void initMic()
{


}

void MainWindow::initWindow()
{
    CAgoraObject::getInstance()->enableAudio(true);
    CAgoraObject::getInstance()->enableVideo(true);
    CAgoraObject::getInstance()->EnableWebSdkInteroperability(true);
    CAgoraObject::getInstance()->SetChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
}
void MainWindow::initGLvideo()
{

//   auto player = ui->openGLWidget;
//   player->setMedia("rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp");
//   player->play();
}

void MainWindow::logger()
{
   std::cout << "text: ";
   qDebug() << ui->roomNum->displayText();
  // std::cout << ui->roomNum->displayText() << std::endl;
   std::cout.flush();
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_entryRoom_clicked()
{
//   pullstream * streamDialog = new pullstream();
//   this->close();
//   streamDialog->show();


    m_strRoomId = ui->roomNum->text();
    m_uppullstream.reset(new pullstream);
    pullstream* receive1 = m_uppullstream.get();
    connect(this,SIGNAL(joinchannel(QMainWindow*,QString,uint)),receive1,SLOT(joinchannel(QMainWindow*,QString,uint)));
    emit joinchannel(this,m_strRoomId,0);


  //  Camera * camera = new Camera();
  //  this->close();
  //  camera->show();
}

