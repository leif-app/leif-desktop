QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app

SOURCES =  ../../../../leif/utils/translation.cpp           \
           ../../../../leif/utils/translatedstring.cpp      \
           tst_translatedstring.cpp

HEADERS = ../../../../leif/utils/translation.h \
          ../../../../leif/utils/translatedstring.h

INCLUDEPATH *= ../../../../leif/utils
