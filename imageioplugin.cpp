#include "imageioplugin.h"

#include "jpegxrhandler.h"

#include <QDebug>

//const static char g_magic[] = {};

ImageIOPlugin::ImageIOPlugin(QObject *parent) :
    QImageIOPlugin(parent)
{
    qDebug() << "plugin loaded";
}

QImageIOPlugin::Capabilities ImageIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    qDebug() << "plugin capabilities" << format;
    QImageIOPlugin::Capabilities cap = 0;

    if (device->isReadable()) {
        device->seek(0);
        QByteArray magic(device->read(4));
        device->seek(0);

        if (magic[0] == 'I' && magic[1] == 'I' && magic[2] == (char)0xbc && magic[3] == (char)0x01) {
            if (device->isReadable())
                cap |= QImageIOPlugin::CanRead;
        }
    }

    if (format == QByteArray("jpegxr") || format == QByteArray("jxr")) {
        if (device->isWritable())
            cap |= QImageIOPlugin::CanWrite;
        if (device->isReadable())
            cap |= QImageIOPlugin::CanRead;
    }


    return cap;
}

QImageIOHandler *ImageIOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    qDebug() << "plugin create" << device << device->objectName();
    JpegXRHandler *handler = new JpegXRHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(JpegXR, ImageIOPlugin)
#endif // QT_VERSION < 0x050000
