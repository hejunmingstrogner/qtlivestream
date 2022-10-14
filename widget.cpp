#include "widget.h"
#include "ui_widget.h"

/*
 * 设置QT界面的样式
*/
void Widget::SetStyle(const QString &qssFile) {
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        QString PaletteColor = qss.mid(20,7);
        qApp->setPalette(QPalette(QColor(PaletteColor)));
        file.close();
    }
    else
    {
        qApp->setStyleSheet("");
    }
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    /*基本设置*/
   // this->SetStyle(":/images/blue.css");     //设置样式表
    this->setWindowIcon(QIcon(":/main.ico")); //设置图标
    this->setWindowTitle("流媒体播放器");

    //构建默认地址
    QStringList listyear;
    listyear<<"rtmp://10.0.0.13:8888/live/video"
           <<"rtmp://58.200.131.2:1935/livetv/cctv14"
           <<"rtsp://admin:vioovi2021@10.0.0.2:554/cam/realmonitor?channel=1&subtype=0"
           ; //列表
    QCompleter *year = new QCompleter(listyear);//构建自动补全器
    ui->lineEdit_rtmp_url->setCompleter(year); //设置自动补全器功能

    //连接拉流线程的图像输出信号
    connect(&thread_laliu,SIGNAL(VideoDataOutput(QImage )),this,SLOT(VideoDataDisplay(QImage )));
    //连接拉流线程的日志信息
    connect(&thread_laliu,SIGNAL(LogSend(QString)),this,SLOT(Log_Display(QString)));
}

Widget::~Widget()
{
    delete ui;
}

//视频刷新显示
void Widget::VideoDataDisplay(QImage image)
{
    ui->widget_display->slotGetOneFrame(image);
}

/*日志显示*/
void Widget::Log_Text_Display(QPlainTextEdit *plainTextEdit_log,QString text)
{
    plainTextEdit_log->insertPlainText(text);
    //移动滚动条到底部
    QScrollBar *scrollbar = plainTextEdit_log->verticalScrollBar();
    if(scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

//日志显示
void Widget::Log_Display(QString text)
{
    Log_Text_Display(ui->plainTextEdit_log,text);
}

//开始拉流
void Widget::on_pushButton_start_clicked()
{
    on_pushButton_stop_play_clicked();
    //设置地址
    thread_laliu.SetRTMPAddr(ui->lineEdit_rtmp_url->text());
    //开始运行线程
    thread_laliu.start();
}


/*
工程: ffmpeg_Laliu
日期: 2021-07-30
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 停止播放
*/
void Widget::on_pushButton_stop_play_clicked()
{
    if(thread_laliu.isRunning())
    {
        thread_laliu.Exit_process();
        thread_laliu.quit();
        thread_laliu.wait();
    }
}

/*
工程: ffmpeg_Laliu
日期: 2021-07-30
作者: DS小龙哥
环境: win10 QT5.12.6 MinGW32
功能: 隐藏日志窗口
*/
void Widget::on_pushButton_no_display_clicked()
{
    log_widge_state=!log_widge_state;
    ui->groupBox->setVisible(log_widge_state);
}
