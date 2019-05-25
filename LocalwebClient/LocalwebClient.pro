#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T17:44:16
#
#-------------------------------------------------

QT       += core gui widgets network multimedia

TARGET = LocalwebClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        main.cpp \
        clientwidget.cpp \
    ../mylogger.cpp \
    ../clientinfo.cpp \
    ../popup.cpp \
    ../settingswidget.cpp \
    registrationwidget.cpp \
    convertionwidget.cpp \
    listdock.cpp \
    clientservice.cpp \
    mainwindow.cpp \
    ../common.cpp \
    ../MY_FTP/MY_FTP/ftp_client.cpp \
../AudioModul/clientside.cpp \
../AudioModul/connectionhandler.cpp \
../AudioModul/iohandler.cpp \
    callingwindow.cpp \
    ../VideoModule/videowidget.cpp \
    ../VideoModule/clientside.cpp \
    ../VideoModule/camera.cpp \
    ../VideoModule/connectionhandler.cpp

HEADERS += \
    ../mylogger.h \
    ../clientinfo.h \
    ../popup.h \
    ../settingswidget.h \
    registrationwidget.h \
    clientwidget.h \
    convertionwidget.h \
    listdock.h \
    clientservice.h \
    mainwindow.h \
    ../common.h \
    ../MY_FTP/MY_FTP/ftp_client.h \
    ../MY_FTP/MY_FTP/ftp_common.h \
../AudioModul/clientside.h \
../AudioModul/connectionhandler.h \
../AudioModul/iohandler.h \
    callingwindow.h \
    ../VideoModule/videowidget.h \
    ../VideoModule/camera.h \
    ../VideoModule/clientside.h \
    ../VideoModule/connectionhandler.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

FORMS += \
    callingwindow.ui
