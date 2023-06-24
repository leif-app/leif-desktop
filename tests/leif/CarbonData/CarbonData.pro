QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app

SOURCES =  tst_carbondata.cpp

HEADERS = ../../../leif/include/carbondata.h

INCLUDEPATH *= ../../../leif/include
