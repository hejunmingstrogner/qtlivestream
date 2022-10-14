#ifndef RTMP_H
#define RTMP_H

#include <QWidget>

namespace Ui {
class rtmp;
}

class rtmp : public QWidget
{
    Q_OBJECT

public:
    explicit rtmp(QWidget *parent = nullptr);
    ~rtmp();

private:
    Ui::rtmp *ui;
    void initVideoLayer();
};

#endif // RTMP_H
