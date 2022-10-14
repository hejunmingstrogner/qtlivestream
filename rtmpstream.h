#ifndef RTMPSTREAM_H
#define RTMPSTREAM_H

#include <QWidget>

namespace Ui {
class rtmpstream;
}

class rtmpstream : public QWidget
{
    Q_OBJECT

public:
    explicit rtmpstream(QWidget *parent = nullptr);
    void initGLvideo();
    ~rtmpstream();

private:
    Ui::rtmpstream *ui;
};

#endif // RTMPSTREAM_H
