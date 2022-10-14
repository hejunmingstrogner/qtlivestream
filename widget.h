#ifndef WIDGET_H
#define WIDGET_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QCompleter>
#include "Thread_FFMPEG_LaLiu.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

//主线程
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void SetStyle(const QString &qssFile);
    void Log_Text_Display(QPlainTextEdit *plainTextEdit_log,QString text);
private slots:
    void Log_Display(QString text);
     void VideoDataDisplay(QImage image);
     void on_pushButton_start_clicked();
     void on_pushButton_stop_play_clicked();

     void on_pushButton_no_display_clicked();

private:
    Ui::Widget *ui;

    bool log_widge_state=true;
};

#endif // WIDGET_H
