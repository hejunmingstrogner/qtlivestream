#include "abstractvideosurface.h"

AbstractVideoSurface::AbstractVideoSurface(QObject* parent):
    QAbstractVideoSurface(parent)
{

}

bool AbstractVideoSurface::present(const QVideoFrame &frame)
{
// 该函数会在摄像头拍摄到每一帧的图片后，自动调用

//  图像处理函数，目标是把图片的格式从 QVideoFrame转换成 QPixmap(或者是能够轻易的转化成QPixmap的其他类)
    QVideoFrame fm = frame;//拷贝构造，内部地址浅拷贝，视频帧依旧存放于不可读取的内存上，需要进行映射
    fm.map(QAbstractVideoBuffer::ReadOnly);//映射视频帧内容，映射到可访问的地址上

    //现在 QVideroFrame，视频帧数据可正常访问了
/*
    QVideoFrame转换成 QPixmap ，我们要寻找转换的方式，一般思路上有2中
    在QVideoFrame里面，寻找： 返回值为 QPixmap，参数为空，函数名类似于 toPixmap 的方法
        或者在QVideoFrame的静态方法里面，寻找返回值为 QPixmap，参数为QVidoFrame的方法
    或者在 QPixmap的构造函数里面，寻找参数为QVideoFrame
        或者 QPixmap的静态方法里面，寻找返回值为 QPixmap，参数为QVideoFrame的方法
    经过这两轮的寻找，没找到可以直接将 QVideoFrame转换成 QPixmap的方法
    QT里面有一个万能的图像类，叫做 QImage
        因为一看构造函数，发现，构建一个QImage只需要图像的首地址，图像的宽度，图像的高度，（图像每一行的字节数），这些数据，任意图像类型，都可以轻易获取
        例如QVideoFrame就能够轻易的获取
            图像首地址：bits
            图像宽度：width
            图像高度：height
            图像每行字节数：bytesPerLine
            图像格式：需求格式为 QImage::Format_RGB32
    QImage::QImage(const uchar *data, int width, int height, Format format, QImageCleanupFunction cleanupFunction = Q_NULLPTR, void *cleanupInfo = Q_NULLPTR)
*/
    //QImage image(fm.bits(),fm.width(),fm.height(),QImage::Format_RGB32); 以下主要是图片格式的转化不同的形式，结果是一样的
    //QImage image(fm.bits(),fm.width(),fm.height(),fm.imageFormatFromPixelFormat(QVideoFrame::Format_RGB32));
    QImage image(fm.bits(),fm.width(),fm.height(),fm.imageFormatFromPixelFormat(fm.pixelFormat()));

    // 注意，摄像头拍摄到的图片，是上下左右相反的，所以我们还需要镜像一下
    image = image.mirrored(1);//表示横向镜像，纵向镜像是默认值
    emit sndImage(image);
}

QList<QVideoFrame::PixelFormat> AbstractVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    Q_UNUSED(type);
//  这个函数在  AbstractVideoSurface构造函数的时候，自动调用，目的是确认AbstractVideoSurface能够支持的像素格式，摄像头拍摄到的图片，他的像素格式是 RGB_3

// 所以，我们要把RGB_32这个像素格式，添加到AbstractVideoSurface支持列表里面了
// 当前函数，返回的就是AbstractVideoSurface所支持的像素格式的列表
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
    // 此处的 operoatr<< 相遇 QList::append方法
}
