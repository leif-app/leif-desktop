#include <QSettings>

#include "leifsettings.h"

class LeifSettingsPrivate
{
    static LeifSettings *Instance;
    static QString CountryKey;
    static QString RegionKey;
    static QString LifeTimeCarbonKey;

    static int toInt(const QVariant &value, int defaultValue);
    static float toFloat(const QVariant &value, float defaultValue);

    friend class LeifSettings;
};

LeifSettings * LeifSettingsPrivate::Instance = nullptr;
QString LeifSettingsPrivate::CountryKey = QStringLiteral("COUNTRY");
QString LeifSettingsPrivate::RegionKey = QStringLiteral("REGION");
QString LeifSettingsPrivate::LifeTimeCarbonKey = QStringLiteral("LIFECARBON");

int LeifSettingsPrivate::toInt(const QVariant &value, int defaultValue)
{
    bool ok = false;
    int iValue = value.toInt(&ok);

    if(!ok)
    {
        return defaultValue;
    }

    return iValue;
}

float LeifSettingsPrivate::toFloat(const QVariant &value, float defaultValue)
{
    bool ok = false;
    float fValue = value.toFloat(&ok);

    if(!ok)
    {
        return defaultValue;
    }

    return fValue;
}

LeifSettings::LeifSettings(QObject *parent /* = nullptr */):
    QObject(parent),
    d{new LeifSettingsPrivate}
{}

LeifSettings::~LeifSettings()
{
    delete d;
    d = nullptr;
}

LeifSettings *LeifSettings::Instance()
{
    if(LeifSettingsPrivate::Instance == nullptr)
    {
        LeifSettingsPrivate::Instance = new LeifSettings;
    }

    return LeifSettingsPrivate::Instance;
}

void LeifSettings::Destroy()
{
    if(LeifSettingsPrivate::Instance != nullptr)
    {
        delete LeifSettingsPrivate::Instance;
        LeifSettingsPrivate::Instance = nullptr;
    }
}

void LeifSettings::saveLocation(const QLocale::Country &country, const QString &regionId)
{
    saveCountry(country);
    saveRegionId(regionId);
}

void LeifSettings::saveCountry(const QLocale::Country &country)
{
    QSettings settings;
    settings.setValue(LeifSettingsPrivate::CountryKey, country);

    emit countryChanged(country);
}

void LeifSettings::saveRegionId(const QString &regionId)
{
    QSettings settings;
    settings.setValue(LeifSettingsPrivate::RegionKey, regionId);

    emit regionIdChanged(regionId);
}

QLocale::Country LeifSettings::country() const
{
    QSettings settings;

    QVariant value = settings.value(LeifSettingsPrivate::CountryKey, QLocale::AnyCountry);
    return static_cast<QLocale::Country>(LeifSettingsPrivate::toInt(value, QLocale::AnyCountry));
}

QString LeifSettings::regionId() const
{
    QSettings settings;

    return settings.value(LeifSettingsPrivate::RegionKey).toString();
}

void LeifSettings::saveLifetimeCarbon(float lifeTime)
{
    QSettings settings;

    settings.setValue(LeifSettingsPrivate::LifeTimeCarbonKey, lifeTime);

    emit lifeTimeCarbonChanged(lifeTime);
}

float LeifSettings::lifeTimeCarbon() const
{
    QSettings settings;

    QVariant value = settings.value(LeifSettingsPrivate::LifeTimeCarbonKey);
    float totalCarbon = LeifSettingsPrivate::toFloat(value, 0);

    return totalCarbon;
}

void LeifSettings::clearLifeTimeCarbon()
{
    saveLifetimeCarbon(0);
}
