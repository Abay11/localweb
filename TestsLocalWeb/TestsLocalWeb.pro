QT += testlib network widgets
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_test.cpp \
#client
../LocalwebClient/clientservice.cpp \
../clientinfo.cpp \
#server
../servernetworkservice.cpp \
    clientservicefordebug.cpp

HEADERS += \
#client
../LocalwebClient/clientservice.h \
  ../clientinfo.h \
#server
../servernetworkservice.h \
    clientservicefordebug.h
