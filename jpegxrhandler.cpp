#include "jpegxrhandler.h"

#include <QImage>
#include <QTemporaryFile>
#include <JXRTest.h>

#include <QDebug>

JpegXRHandler::JpegXRHandler()
{
}

bool JpegXRHandler::canRead() const
{
    return device()->isReadable();
}

bool JpegXRHandler::read(QImage *image)
{
    QIODevice *file = device();
    qDebug() << "Read" << file << file->isOpen() << file->isReadable() << file->isSequential();
    QTemporaryFile tempfile("XXXXXXX.jxr");
    tempfile.open();
    tempfile.write(file->readAll());
    //qDebug() << tempfile.fileName();

    ERR err = WMP_errSuccess;

    PKFactory* pFactory = NULL;
    PKCodecFactory* pCodecFactory = NULL;
    PKImageDecode* pDecoder = NULL;
    PKFormatConverter* pConverter = NULL;

    PKRect rect = {0,0,0,0};
    U8 *pixelBuffer = NULL;

    Call(PKCreateFactory(&pFactory, PK_SDK_VERSION));
    Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
    Call(pCodecFactory->CreateDecoderFromFile(tempfile.fileName().toUtf8(), &pDecoder));

    PKPixelFormatGUID pixelFormatGUID;
    Call(pDecoder->GetPixelFormat(pDecoder, &pixelFormatGUID));

    if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat24bppBGR)) {
        qDebug() << "GUID_PKPixelFormat24bppBGR";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat24bppRGB)) {
        qDebug() << "GUID_PKPixelFormat24bppRGB";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppBGR)) {
        qDebug() << "GUID_PKPixelFormat32bppBGR";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppBGRA)) {
        qDebug() << "GUID_PKPixelFormat32bppBGRA";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppPBGRA)) {
        qDebug() << "GUID_PKPixelFormat32bppPBGRA";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppRGB)) {
        qDebug() << "GUID_PKPixelFormat32bppRGB";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppRGBA)) {
        qDebug() << "GUID_PKPixelFormat32bppRGBA";
    } else if (IsEqualGUID(pixelFormatGUID, GUID_PKPixelFormat32bppPRGBA)) {
        qDebug() << "GUID_PKPixelFormat32bppPRGBA";
    } else {
        qDebug() << hex << pixelFormatGUID.Data1 << pixelFormatGUID.Data2 << pixelFormatGUID.Data3 << pixelFormatGUID.Data4;
    }

    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;

    qDebug() << "Initialize converter";

    //Create color converter
    Call(pCodecFactory->CreateFormatConverter(&pConverter));
    Call(pConverter->Initialize(pConverter, pDecoder, NULL, GUID_PKPixelFormat32bppBGR));

    rect.Width = (I32)(pDecoder->WMP.wmiI.cThumbnailWidth);
    rect.Height = (I32)(pDecoder->WMP.wmiI.cThumbnailHeight);


    PKAllocAligned((void **)&pixelBuffer, rect.Width * 4 * rect.Height, 8);

    qDebug() << "call copy";

    Call(pConverter->Copy(pConverter, &rect, pixelBuffer, rect.Width * 4 ));

//    for (int i = 0; i < 5; i++) {
//        qDebug() << pixelBuffer[i*4] << pixelBuffer[i*4+1] << pixelBuffer[i*4+2] << pixelBuffer[i*4+3];
//    }

    for (int i = 0; i < rect.Width * rect.Height; i++) {
//        U8 tmp = pixelBuffer[i*4];
//        //pixelBuffer[i*4] = pixelBuffer[i*4+3];
//        pixelBuffer[i*4+3] = tmp;

//        tmp = pixelBuffer[i*4+1];
//        pixelBuffer[i*4+1] = pixelBuffer[i*4+2];
//        pixelBuffer[i*4+2] = tmp;
        pixelBuffer[i*4+3] = 0xff;// - pixelBuffer[i*4+3];
    }

//    for (int i = 0; i < 20; i++) {
//        qDebug() << pixelBuffer[i*4] << pixelBuffer[i*4+1] << pixelBuffer[i*4+2] << pixelBuffer[i*4+3];
//    }

    {
        QImage newimage((uchar*) pixelBuffer, rect.Width, rect.Height, QImage::Format_RGB32);
        *image = newimage;
    }

    //PKFreeAligned((void **)&pixelBuffer);
    return true;
  Cleanup:
    qDebug() << "Error" << err;
    return false;

}

bool JpegXRHandler::write(const QImage &image)
{
    QImage im;
    if (image.format() != QImage::Format_ARGB32) {
        im = image.convertToFormat(QImage::Format_ARGB32);
    } else {
        im = image;
    }

    // setup paramter
    CWMIStrCodecParam wmiSCP;
    memset(&wmiSCP, 0, sizeof(wmiSCP));
    wmiSCP.bVerbose = FALSE;
    wmiSCP.cfColorFormat = YUV_444;
    wmiSCP.bdBitDepth = BD_LONG;
    wmiSCP.bfBitstreamFormat = FREQUENCY;
    wmiSCP.bProgressiveMode = TRUE;
    wmiSCP.olOverlap = OL_ONE;
    wmiSCP.cNumOfSliceMinus1H = wmiSCP.cNumOfSliceMinus1V = 0;
    wmiSCP.sbSubband = SB_ALL;
    wmiSCP.uAlphaMode = 0;
    wmiSCP.uiDefaultQPIndex = 1;
    wmiSCP.uiDefaultQPIndexAlpha = 1;

    //wmiSCP.uAlphaMode = 2;

    QIODevice *file = device();
    qDebug() << "Write" << file << file->isOpen() << file->isWritable();

    //QTemporaryFile tempfile("XXXXXXX.jxr");
    QFile tempfile("/Users/yasu/Desktop/tmp.jxr");
    tempfile.open(QIODevice::ReadWrite);

    ERR err = WMP_errSuccess;

    PKFactory* pFactory = NULL;
    struct WMPStream* pEncodeStream = NULL;
    PKCodecFactory* pCodecFactory = NULL;
    //PKCodecFactory* pTestFactory = NULL;
    PKImageEncode* pEncoder = NULL;

    Call(PKCreateFactory(&pFactory, PK_SDK_VERSION));
    Call(pFactory->CreateStreamFromFilename(&pEncodeStream, tempfile.fileName().toUtf8(), "wb"));

    //================================
    Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
    Call(pCodecFactory->CreateCodec(&IID_PKImageWmpEncode, (void **) &pEncoder));

    //----------------------------------------------------------------
    //Call(PKCreateTestFactory(&pTestFactory, WMP_SDK_VERSION));


    {

        PKRect rect = {0, 0, 0, 0};
        rect.Width = im.width();
        rect.Height = im.height();

        if (wmiSCP.cNumOfSliceMinus1H == 0 && wmiSCP.uiTileY[0] > 0)
        {
            // # of horizontal slices, rounded down by half tile size.
            U32 uTileY = wmiSCP.uiTileY[0] * MB_HEIGHT_PIXEL;
            wmiSCP.cNumOfSliceMinus1H = (U32) rect.Height < (uTileY >> 1) ? 0 :
                                                                            (rect.Height + (uTileY >> 1)) / uTileY - 1;
        }
        if (wmiSCP.cNumOfSliceMinus1V == 0 && wmiSCP.uiTileX[0] > 0)
        {
            // # of vertical slices, rounded down by half tile size.
            U32 uTileX = wmiSCP.uiTileX[0] * MB_HEIGHT_PIXEL;
            wmiSCP.cNumOfSliceMinus1V = (U32) rect.Width < (uTileX >> 1) ? 0 :
                                                                           (rect.Width + (uTileX >> 1)) / uTileX - 1;
        }

        Call(pEncoder->Initialize(pEncoder, pEncodeStream, &wmiSCP, sizeof(wmiSCP)));
        pEncoder->WMP.wmiSCP.uiDefaultQPIndex = 82;

//        qDebug() << "Encoder Initialized";

        if(pEncoder->WMP.wmiSCP.uAlphaMode == 2)
            pEncoder->WMP.wmiSCP_Alpha.uiDefaultQPIndex = wmiSCP.uiDefaultQPIndexAlpha;

        Call(pEncoder->SetPixelFormat(pEncoder, GUID_PKPixelFormat32bppBGR));

//        qDebug() << "SetPixelFormat";

        Call(pEncoder->SetSize(pEncoder, rect.Width, rect.Height));
        Call(pEncoder->SetResolution(pEncoder, 100, 100));

       // qDebug() << "SetSize";

        //qDebug() << "before write pixels" << rect.Height << rect.Width << im.bytesPerLine() << im.byteCount();

        Call(pEncoder->WritePixels(pEncoder, rect.Height, im.bits(), im.bytesPerLine()));
    }

    Call(pEncoder->Terminate(pEncoder));
    Call(pEncoder->Release(&pEncoder));

    //qDebug() << tempfile.size();

    file->write(tempfile.readAll());

    return false;
Cleanup:
    qDebug() << "Error" << err;
    return false;

}
