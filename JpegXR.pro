#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T13:53:32
#
#-------------------------------------------------

QT       += core gui

TARGET = JpegXR
TEMPLATE = lib
CONFIG += plugin

#DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats
DESTDIR = ../SimpleImageViewer/imageformats

INCLUDEPATH += jxrlib/jxrgluelib jxrlib/jxrtestlib jxrlib/image/sys jxrlib/image/encode jxrlib/image/decode jxrlib/image/x86 jxrlib/common/include
DEFINES += __ANSI__ DISABLE_PERF_MEASUREMENT

SOURCES += imageioplugin.cpp \
    jpegxrhandler.cpp \
    jxrlib/jxrgluelib/JXRGlue.c \
    jxrlib/jxrgluelib/JXRGlueJxr.c \
    jxrlib/jxrgluelib/JXRGluePFC.c \
    jxrlib/jxrgluelib/JXRMeta.c \
    jxrlib/jxrtestlib/JXRTest.c \
    jxrlib/jxrtestlib/JXRTestBmp.c \
    jxrlib/jxrtestlib/JXRTestHdr.c \
    jxrlib/jxrtestlib/JXRTestPnm.c \
    jxrlib/jxrtestlib/JXRTestTif.c \
    jxrlib/jxrtestlib/JXRTestYUV.c \
    jxrlib/image/decode/decode.c \
    jxrlib/image/decode/JXRTranscode.c \
    jxrlib/image/decode/postprocess.c \
    jxrlib/image/decode/segdec.c \
    jxrlib/image/decode/strdec_x86.c \
    jxrlib/image/decode/strdec.c \
    jxrlib/image/decode/strInvTransform.c \
    jxrlib/image/decode/strPredQuantDec.c \
    jxrlib/image/encode/encode.c \
    jxrlib/image/encode/segenc.c \
    jxrlib/image/encode/strenc_x86.c \
    jxrlib/image/encode/strenc.c \
    jxrlib/image/encode/strFwdTransform.c \
    jxrlib/image/encode/strPredQuantEnc.c \
    jxrlib/image/sys/adapthuff.c \
    jxrlib/image/sys/image.c \
    jxrlib/image/sys/perfTimerANSI.c \
    jxrlib/image/sys/strcodec.c \
    jxrlib/image/sys/strPredQuant.c \
    jxrlib/image/sys/strTransform.c

HEADERS += imageioplugin.h \
    jpegxrhandler.h \
    jxrlib/jxrgluelib/JXRGlue.h \
    jxrlib/jxrgluelib/JXRMeta.h \
    jxrlib/jxrtestlib/JXRTest.h \
    jxrlib/image/decode/decode.h \
    jxrlib/image/encode/encode.h \
    jxrlib/image/sys/ansi.h \
    jxrlib/image/sys/common.h \
    jxrlib/image/sys/perfTimer.h \
    jxrlib/image/sys/strcodec.h \
    jxrlib/image/sys/strTransform.h \
    jxrlib/image/sys/windowsmediaphoto.h \
    jxrlib/image/sys/xplatform_image.h
OTHER_FILES += JpegXR.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
