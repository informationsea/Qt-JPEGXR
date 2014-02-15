#ifndef IMAGEIOPLUGIN_H
#define IMAGEIOPLUGIN_H

#include <QImageIOPlugin>


class ImageIOPlugin : public QImageIOPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "JpegXR.json")
#endif // QT_VERSION >= 0x050000

public:
    ImageIOPlugin(QObject *parent = 0);

    virtual Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    virtual QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

#endif // IMAGEIOPLUGIN_H
