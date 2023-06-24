#include <QTimer>
#include <QDebug>

#include "carbonservice.h"
#include "settingsservice.h"
#include "powerfactory.h"

#include "plugin/carbonpluginmanager.h"

#include "interfaces/IPower.h"

#include "log/log.h"

class CarbonServicePrivate
{
private:
    float session;
    float lifetime;
    CarbonUsageLevel usageLevel;
    ChargeForecast chargeForecast;
    QScopedPointer<IPower> powerInfo;
    SettingsService *settings;
    CarbonData cachedData;

    QTimer calculateTimer;
    
    friend class CarbonService;
};

CarbonService::CarbonService(SettingsService *settings, QObject *parent)
    : QObject{parent},
      d{new CarbonServicePrivate}
{
    d->session = 0.0;
    d->lifetime = 0.0;
    d->usageLevel = CarbonUsageLevel::VeryHigh;
    d->chargeForecast = ChargeForecast::ChargeWhenNeeded;
    d->powerInfo.reset(PowerFactory::getPowerInterface(settings).release());
    d->settings = settings;

    if(d->settings != nullptr)
        setLifetimeCarbon(settings->lifeTimeCarbon());

    QTimer *checkTimer = new QTimer(this);
    checkTimer->setInterval(1000 * 60);
    checkTimer->setSingleShot(false);
    connect(checkTimer, &QTimer::timeout, this, &CarbonService::calculateCarbon);
    checkTimer->start();
    calculateCarbon();
}

CarbonService::~CarbonService()
{
    if(d->settings != nullptr)
        d->settings->saveLifetimeCarbon(lifetimeCarbon());

    d->settings = nullptr;
}

float CarbonService::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->session;
}

float CarbonService::lifetimeCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->lifetime;
}

CarbonUsageLevel CarbonService::carbonUsageLevel() const
{
    Q_ASSERT(d != nullptr);

    return d->usageLevel;
}

ChargeForecast CarbonService::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    return d->chargeForecast;
}

void CarbonService::clearStats()
{
    setSessionCarbon(0);
    setLifetimeCarbon(0);
}

void CarbonService::calculateCarbon()
{
    DBG_CALLED;
    Q_ASSERT(d != nullptr);

    if(d->settings == nullptr)
    {
        ERR("Can't calculate carbon, SettingsService not available.");
        return;
    }

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        ERR("Can't calcuate carbon, CarbonPluginManager not available.");
        return;
    }

    float powerDraw = d->powerInfo->powerDrawInWatts();
    if(powerDraw < 0)
    {
        WRN("Power draw seems to be negative. We will ignore it and set it to zero.");
        powerDraw = 0;
    }

    CarbonData data = d->cachedData;
    if(isOutOfDate(data))
    {
        DBG("Cached expired or invalid. Retrieving carbon data from plugin.");
        data = manager->carbonPerKiloWatt(d->settings->country(), d->settings->regionId());
    }

    INF(QString("Current power draw: %1.").arg(powerDraw));
    INF(QString("Received carbon data is: %1.").arg(data.isValid ? QStringLiteral("valid") : QStringLiteral("invalid")));

    if(data.isValid)
    {
        DBG(QString("Carbon usage from: %1.").arg(data.validFrom.toString()));
        DBG(QString("Carbon usage to: %1.").arg(data.validTo.toString()));
        DBG(QString("Carbon usage is: %1.").arg(data.co2PerkWhNow));

        float carbon = (powerDraw * static_cast<float>(data.co2PerkWhNow)) / (60*1000);
        DBG(QString("Calculated carbon usage is: %1").arg(carbon));

        if(carbon < 0)
        {
            WRN("Calculated carbon usage seems ot be negative. Correcting it now.");
            carbon *= -1;
        }

        setSessionCarbon(sessionCarbon() + carbon);
        setLifetimeCarbon(lifetimeCarbon() + carbon);
        setCarbonUsageLevel(calculateUsageLevel(data.co2PerkWhNow));
        setChargeForecast(calculateChargeForecast(data));
        d->cachedData = data;
    }
    else
    {
        ERR(QString("Carbon data error: %1").arg(data.errorString));
    }
}

CarbonUsageLevel CarbonService::calculateUsageLevel(int co2PerkWh)
{
    DBG_CALLED;
    
    CarbonUsageLevel newLevel = CarbonUsageLevel::VeryHigh;

    if(co2PerkWh <= 49)
    {
        newLevel = CarbonUsageLevel::VeryLow;
    }
    else if(co2PerkWh <= 129)
    {
        newLevel = CarbonUsageLevel::Low;
    }
    else if(co2PerkWh <= 209)
    {
        newLevel = CarbonUsageLevel::Medium;
    }
    else if(co2PerkWh <= 310)
    {
        newLevel = CarbonUsageLevel::High;
    }
    else
    {
        newLevel = CarbonUsageLevel::VeryHigh;
    }

    return newLevel;
}

void CarbonService::setSessionCarbon(float newSessionCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->session != newSessionCarbon)
    {
        d->session = newSessionCarbon;
        emit sessionCarbonChanged();
    }
}

void CarbonService::setLifetimeCarbon(float newLifetimeCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->lifetime != newLifetimeCarbon)
    {
        d->lifetime = newLifetimeCarbon;
        emit lifetimeCarbonChanged();
    }
}

void CarbonService::setCarbonUsageLevel(CarbonUsageLevel newLevel)
{
    Q_ASSERT(d != nullptr);

    INF(QString("New carbon usage level is: %1").arg(static_cast<int>(newLevel)));

    if(d->usageLevel != newLevel)
    {
        d->usageLevel = newLevel;
        emit carbonUsageLevelChanged();
    }
}

void CarbonService::setChargeForecast(ChargeForecast newChargeForecast)
{
    Q_ASSERT(d != nullptr);

    INF(QString("New charge forecast: %1.").arg(static_cast<int>(newChargeForecast)));

    if(d->chargeForecast != newChargeForecast)
    {
        d->chargeForecast = newChargeForecast;
        emit chargeForecastChanged();
    }
}

/* static */
bool CarbonService::isOutOfDate(const CarbonData &data)
{
    if(!data.isValid)
        return true;

    return QDateTime::currentDateTime() > data.validTo;
}

/* static */
ChargeForecast CarbonService::calculateChargeForecast(const CarbonData &data)
{
    if(!data.isValid)
    {
        return ChargeForecast::ChargeWhenNeeded;
    }

    int now = data.co2PerkWhNow;
    int next = data.co2PerkWhNext;
    int later = data.co2PerkWhLater;

    // Now is less than anytime in near future
    if(now <= next && now < later)
    {
        return ChargeForecast::ChargeNow;
    } else if(now > next)
    {
        return ChargeForecast::ChargeIn30;
    } else if(now > later)
    {
        return ChargeForecast::ChargeIn60;
    }

    return ChargeForecast::ChargeWhenNeeded;
}
