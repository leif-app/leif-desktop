QT += quick widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        controllers/carboncontroller.cpp \
        controllers/settingscontroller.cpp \
        log/consolelogger.cpp \
        log/filelogger.cpp \
        log/iodevicelogger.cpp \
        log/logfilterbase.cpp \
        log/logfilterbyfile.cpp \
        log/loggerbase.cpp \
        log/logmanager.cpp \
        log/logsystem.cpp \
        log/predictivelogger.cpp \
        models/countrymodel.cpp \
        models/regionmodel.cpp \
        controllers/trayiconcontroller.cpp \
        plugin/carbonplugin.cpp \
        main.cpp \
        plugin/carbonpluginmanager.cpp \
        powerinfobase.cpp \
        services/carbonservice.cpp \
        services/settingsservice.cpp \
        trayicon.cpp \
        utils/carbonplugindata.cpp \
        utils/qmlwarninglogger.cpp \
        utils/territory.cpp \
        utils/translatedstring.cpp \
        utils/translation.cpp
		
mac: SOURCES += \
                mac/powerinfo.cpp \
                mac/powerfactory_mac.cpp

win32: SOURCES += \
                win/powerinfo.cpp \
                win/powerfactory_win.cpp
			      

RESOURCES += qml.qrc

TRANSLATIONS += \
    lang/leif_en_GB.ts \
    lang/leif_de_DE.ts

CONFIG += lrelease
CONFIG += embed_translations qmltypes

INCLUDEPATH += include/ models/ controllers/ services/

QML_IMPORT_NAME = Leif
QML_IMPORT_MAJOR_VERSION = 1

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    controllers/carboncontroller.h \
    controllers/settingscontroller.h \
    include/carbonusagelevel.h \
    include/chargeforecast.h \
    log/consolelogger.h \
    log/filelogger.h \
    log/ilogger.h \
    log/ilogmanager.h \
    log/iodevicelogger.h \
    log/log.h \
    log/log_internal.h \
    log/logfilterbase.h \
    log/logfilterbyfile.h \
    log/loggerbase.h \
    log/logmanager.h \
    log/logsystem.h \
    log/messagetype.h \
    log/predictivelogger.h \
    models/countrymodel.h \
    models/regionmodel.h \
    controllers/trayiconcontroller.h \
    plugin/carbonplugin.h \
    include/carbondata.h \
    include/interfaces/IDataProvider.h \
    include/interfaces/IPower.h \
    main.h \
    plugin/carbonpluginmanager.h \
    powerfactory.h \
    powerinfobase.h \
    services/carbonservice.h \
    services/settingsservice.h \
    trayicon.h \
    utils/carbonplugindata.h \
    utils/qmlwarninglogger.h \
    utils/territory.h \
    utils/translatedstring.h \
    utils/translation.h
	
mac: HEADERS += mac/powerinfo.h

win32: HEADERS += win/powerinfo.h

win32: LIBS *= PowrProf.lib

mac: LIBS += -framework IOKit
mac: LIBS += -framework CoreFoundation

include('pri/appinfo.pri')
win32: include('pri/predeps.pri')
win32: include('pri/win.pri')
mac: include('pri/mac.pri')
