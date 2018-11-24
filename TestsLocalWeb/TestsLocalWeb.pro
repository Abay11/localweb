QT += testlib network widgets
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_test.cpp \
#client
../LocalwebClient/clientservice.cpp \
../clientinfo.cpp \
#server
../mywidget.cpp \
../servernetworkservice.cpp


HEADERS += \
#client
../LocalwebClient/clientservice.h \
  ../clientinfo.h \
#server
../mywidget.h \
../servernetworkservice.h
