/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <carbonmodel.h>
#include <countrymodel.h>
#include <regionmodel.h>
#include <settingsmodel.h>

void qml_register_types_Leif()
{
    qmlRegisterTypesAndRevisions<CarbonModel>("Leif", 1);
    qmlRegisterTypesAndRevisions<CountryModel>("Leif", 1);
    qmlRegisterTypesAndRevisions<RegionModel>("Leif", 1);
    qmlRegisterTypesAndRevisions<SettingsModel>("Leif", 1);
    qmlRegisterModule("Leif", 1, 0);
}

static const QQmlModuleRegistration registration("Leif", 1, qml_register_types_Leif);
