#ifndef ABSTRACTVIDEOSURFACE_H
#define ABSTRACTVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>

class AbstractVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    AbstractVideoSurface(QObject* parent = NULL);
    virtual bool present(const QVideoFrame &frame);
    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;

signals:
    void sndImage(QImage);


};

#endif // ABSTRACTVIDEOSURFACE_H
