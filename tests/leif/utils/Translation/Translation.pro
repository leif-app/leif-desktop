QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app

SOURCES =  ../../../../leif/utils/translation.cpp      \
           tst_translation.cpp

HEADERS = ../../../../leif/utils/translation.h

INCLUDEPATH *= ../../../../leif/utils
