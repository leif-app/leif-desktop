#include "settingsservice.h"
#include "settingscontroller.h"

SettingsController::SettingsController(SettingsService *settings, QObject *parent)
    : QObject{parent}
{
    d.settings = settings;

    if(d.settings != nullptr)
    {
        connect(d.settings, &SettingsService::countryChanged, this, [=]() { emit countryChanged();});
        connect(d.settings, &SettingsService::regionIdChanged, this, [=]() { emit regionIdChanged();});
        connect(d.settings, &SettingsService::lifeTimeCarbonChanged, this, [=]() { emit lifetimeCarbonChanged();});
    }
}

QLocale::Country SettingsController::country() const
{
    if(d.settings == nullptr)
        return QLocale::AnyCountry;

    return d.settings->country();
}

void SettingsController::setCountry(const QLocale::Country &newCountry)
{
    if(d.settings == nullptr)
        return;

    if(d.settings->country() != newCountry)
        d.settings->saveCountry(newCountry);
}

QString SettingsController::regionId() const
{
    if(d.settings == nullptr)
        return QString();

    return d.settings->regionId();
}

void SettingsController::setRegionId(const QString &newRegionId)
{
    if(d.settings == nullptr)
        return;

    if(d.settings->regionId() != newRegionId)
        d.settings->saveRegionId(newRegionId);
}

float SettingsController::lifetimeCarbon() const
{
    if(d.settings == nullptr)
        return 0;

    return d.settings->lifeTimeCarbon();
}

void SettingsController::setLifetimeCarbon(float newLifetimeCarbon)
{
    if(d.settings == nullptr)
        return;

    if(d.settings->lifeTimeCarbon() != newLifetimeCarbon)
        d.settings->saveLifetimeCarbon(newLifetimeCarbon);
}
