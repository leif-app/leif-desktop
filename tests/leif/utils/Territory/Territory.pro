QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app

SOURCES =  ../../../../leif/utils/translation.cpp           \
           ../../../../leif/utils/translatedstring.cpp      \
           ../../../../leif/utils/territory.cpp             \
           tst_territory.cpp

HEADERS = ../../../../leif/utils/translation.h \
          ../../../../leif/utils/translatedstring.h \
          ../../../../leif/utils/territory.h

INCLUDEPATH *= ../../../../leif/utils
