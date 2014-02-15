#ifndef JPEGXRHANDLER_H
#define JPEGXRHANDLER_H

#include <QImageIOHandler>

class JpegXRHandler : public QImageIOHandler
{
public:
    JpegXRHandler();

    virtual bool canRead() const;
    virtual bool read(QImage * image);
    virtual bool write(const QImage & image);
};

#endif // JPEGXRHANDLER_H
