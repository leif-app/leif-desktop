QT -= gui
QT += network

TEMPLATE = lib
DEFINES += UK_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    uk.cpp \
    utilities.cpp

HEADERS += \
    uk_global.h \
    uk.h \
    utilities.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ../../leif/include

DISTFILES += \
    uk.json

procopyplugin.target = ..\..\leif\release\plugins
precopyplugin.commands = IF NOT EXIST $$procopyplugin.target MD $$procopyplugin.target

copyplugin.commands = for %I in (uk.json release\uk.dll) do copy %I $$procopyplugin.target
copyplugin.depends = precopyplugin

QMAKE_EXTRA_TARGETS += copyplugin precopyplugin

QMAKE_CLEAN *= ../../leif/release/plugins/uk.json \
               ../../leif/release/plugins/uk.dll
