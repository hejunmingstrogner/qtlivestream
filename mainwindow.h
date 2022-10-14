#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QCamera>
#include <QVideoWidget>
//#include <QCameraViewfinder>
//#include <QCameraImageCapture>
#include "pullstream.h"
class QCameraViewfinder;
#include <QDebug>
//#include <QtOpenGLWidgets/qopenglwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
   void logger();
   void on_entryRoom_clicked();


public:
   QCamera *camera;
 //  QCameraViewfinder *cameraViewFinder;
//   QCameraImageCapture *cameraImageCapture;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void joinchannel(QMainWindow* pMainWnd,const QString &qsChannel,uint uid);

private:
    Ui::MainWindow *ui;
    void initGLvideo();
    void initWindow();
//   QCameraViewfinder * viewfinder;
    QString     m_strRoomId;
    std::unique_ptr<pullstream> m_uppullstream;
 //   OpenGLWidget *glWidget;
};
#endif // MAINWINDOW_H
