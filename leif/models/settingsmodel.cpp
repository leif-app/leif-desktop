#include "leifsettings.h"
#include "settingsmodel.h"

SettingsModel::SettingsModel(QObject *parent)
    : QObject{parent}
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings != nullptr)
    {
        connect(settings, &LeifSettings::countryChanged, this, [=]() { emit countryChanged();});
        connect(settings, &LeifSettings::regionIdChanged, this, [=]() { emit regionIdChanged();});
        connect(settings, &LeifSettings::lifeTimeCarbonChanged, this, [=]() { emit lifetimeCarbonChanged();});
    }
}

QLocale::Country SettingsModel::country() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return QLocale::AnyCountry;
    }

    return settings->country();
}

void SettingsModel::setCountry(const QLocale::Country &newCountry)
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->country() != newCountry)
    {
        settings->saveCountry(newCountry);
    }
}

QString SettingsModel::regionId() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return QString();
    }

    return settings->regionId();
}

void SettingsModel::setRegionId(const QString &newRegionId)
{
    qDebug("setRegionId");
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->regionId() != newRegionId)
    {
        settings->saveRegionId(newRegionId);
    }
}

float SettingsModel::lifetimeCarbon() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return 0;
    }

    return settings->lifeTimeCarbon();
}

void SettingsModel::setLifetimeCarbon(float newLifetimeCarbon)
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->lifeTimeCarbon() != newLifetimeCarbon)
    {
        settings->saveLifetimeCarbon(newLifetimeCarbon);
    }
}
