#include <QSettings>

#include "settingsservice.h"
#include "log/log.h"

class LeifSettingsPrivate
{
    constexpr static const char* CountryKey {"COUNTRY"};
    constexpr static const char* RegionKey {"REGION"};
    constexpr static const char* LifeTimeCarbonKey {"LIFECARBON"};
    constexpr static const char* AvgDischargeRateKey {"AVGDISCHARGERATE"};

    static int toInt(const QVariant &value, int defaultValue);
    static float toFloat(const QVariant &value, float defaultValue);
    
    friend class SettingsService;
};

int LeifSettingsPrivate::toInt(const QVariant &value, int defaultValue)
{
    bool ok = false;
    int iValue = value.toInt(&ok);

    if(!ok)
    {
        WRN(QString("Could not convert %1 to an integer.").arg(value.toString()));
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
        WRN(QString("Could not convert %1 to a float.").arg(value.toString()));
        return defaultValue;
    }

    return fValue;
}

SettingsService::SettingsService(QObject *parent /* = nullptr */):
    QObject {parent}
{}

void SettingsService::saveLocation(const QLocale::Country &country, const QString &regionId)
{
    saveCountry(country);
    saveRegionId(regionId);
}

void SettingsService::saveCountry(const QLocale::Country &country)
{
    QSettings settings;
    settings.setValue(LeifSettingsPrivate::CountryKey, country);

    emit countryChanged(country);
}

void SettingsService::saveRegionId(const QString &regionId)
{
    QSettings settings;
    settings.setValue(LeifSettingsPrivate::RegionKey, regionId);

    emit regionIdChanged(regionId);
}

void SettingsService::saveAverageDischargeRate(int averageDischargeRate)
{
    QSettings settings;
    settings.setValue(LeifSettingsPrivate::AvgDischargeRateKey,
                      averageDischargeRate);

    emit averageDischargeRateChanged(averageDischargeRate);
}

QLocale::Country SettingsService::country() const
{
    QSettings settings;

    QVariant value = settings.value(LeifSettingsPrivate::CountryKey, QLocale::AnyCountry);
    return static_cast<QLocale::Country>(LeifSettingsPrivate::toInt(value, QLocale::AnyCountry));
}

QString SettingsService::regionId() const
{
    QSettings settings;

    return settings.value(LeifSettingsPrivate::RegionKey).toString();
}

int SettingsService::averageDischargeRate() const
{
    QSettings settings;

    return settings.value(LeifSettingsPrivate::AvgDischargeRateKey).toInt();
}

void SettingsService::saveLifetimeCarbon(float lifeTime)
{
    QSettings settings;

    settings.setValue(LeifSettingsPrivate::LifeTimeCarbonKey, lifeTime);

    emit lifeTimeCarbonChanged(lifeTime);
}

float SettingsService::lifeTimeCarbon() const
{
    QSettings settings;

    QVariant value = settings.value(LeifSettingsPrivate::LifeTimeCarbonKey);
    float totalCarbon = LeifSettingsPrivate::toFloat(value, 0);

    return totalCarbon;
}

void SettingsService::clearLifeTimeCarbon()
{
    saveLifetimeCarbon(0);
}
