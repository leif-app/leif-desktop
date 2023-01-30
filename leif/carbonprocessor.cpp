#include <QTimer>
#include <QDebug>

#include "carbonprocessor.h"
#include "leifsettings.h"
#include "powerfactory.h"

#include "plugin/carbonpluginmanager.h"

#include "interfaces/IPower.h"

#include "log/log.h"

class CarbonProcessorPrivate
{
    static CarbonProcessor *Instance;

    float session;
    float lifetime;
    CarbonProcessor::CarbonUsageLevel usageLevel;
    CarbonProcessor::ChargeForecast chargeForecast;
    IPower *powerInfo;
    CarbonData cachedData;

    QTimer calculateTimer;

    friend class CarbonProcessor;
};

CarbonProcessor * CarbonProcessorPrivate::Instance = nullptr;

CarbonProcessor::CarbonProcessor(QObject *parent)
    : QObject{parent},
      d{new CarbonProcessorPrivate}
{
    d->session = 0.0;
    d->lifetime = 0.0;
    d->usageLevel = VeryHigh;
    d->chargeForecast = ChargeWhenNeeded;
    d->powerInfo = PowerFactory::getPowerInterface();

    LeifSettings *settings = LeifSettings::Instance();
    if(settings != nullptr)
    {
        setLifetimeCarbon(settings->lifeTimeCarbon());
    }

    QTimer *checkTimer = new QTimer(this);
    checkTimer->setInterval(1000 * 60);
    checkTimer->setSingleShot(false);
    connect(checkTimer, &QTimer::timeout, this, &CarbonProcessor::calculateCarbon);
    checkTimer->start();
    calculateCarbon();
}

CarbonProcessor::~CarbonProcessor()
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings != nullptr)
    {
        settings->saveLifetimeCarbon(lifetimeCarbon());
    }

    delete d->powerInfo;
    d->powerInfo = nullptr;

    delete d;
    d = nullptr;
}

CarbonProcessor *CarbonProcessor::Instance()
{
    if(CarbonProcessorPrivate::Instance == nullptr)
    {
        CarbonProcessorPrivate::Instance = new CarbonProcessor;
    }

    return CarbonProcessorPrivate::Instance;
}

void CarbonProcessor::Destroy()
{
    if(CarbonProcessorPrivate::Instance != nullptr)
    {
        delete CarbonProcessorPrivate::Instance;
        CarbonProcessorPrivate::Instance = nullptr;
    }
}

float CarbonProcessor::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->session;
}

float CarbonProcessor::lifetimeCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->lifetime;
}

CarbonProcessor::CarbonUsageLevel CarbonProcessor::carbonUsageLevel() const
{
    Q_ASSERT(d != nullptr);

    return d->usageLevel;
}

CarbonProcessor::ChargeForecast CarbonProcessor::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    return d->chargeForecast;
}

void CarbonProcessor::clearStats()
{
    setSessionCarbon(0);
    setLifetimeCarbon(0);
}

void CarbonProcessor::calculateCarbon()
{
    DBG_CALLED;
    Q_ASSERT(d != nullptr);

    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        ERR("Can't calculate carbon, LeifSettings not available.");
        return;
    }

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        ERR("Can't calcuate carbon, CarbonPluginManager not available.");
        return;
    }

    float powerDraw = d->powerInfo->powerDrawInWatts();
    CarbonData data = d->cachedData;
    if(isOutOfDate(data))
    {
        DBG("Cached expired or invalid. Retrieving carbon data from plugin.");
        data = manager->carbonPerKiloWatt(settings->country(), settings->regionId());
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

CarbonProcessor::CarbonUsageLevel CarbonProcessor::calculateUsageLevel(int co2PerkWh)
{
    DBG_CALLED;

    CarbonUsageLevel newLevel = CarbonProcessor::VeryHigh;

    if(co2PerkWh <= 49)
    {
        newLevel = CarbonProcessor::VeryLow;
    }
    else if(co2PerkWh <= 129)
    {
        newLevel = CarbonProcessor::Low;
    }
    else if(co2PerkWh <= 209)
    {
        newLevel = CarbonProcessor::Medium;
    }
    else if(co2PerkWh <= 310)
    {
        newLevel = CarbonProcessor::High;
    }
    else
    {
        newLevel = CarbonProcessor::VeryHigh;
    }

    return newLevel;
}

void CarbonProcessor::setSessionCarbon(float newSessionCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->session != newSessionCarbon)
    {
        d->session = newSessionCarbon;
        emit sessionCarbonChanged();
    }
}

void CarbonProcessor::setLifetimeCarbon(float newLifetimeCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->lifetime != newLifetimeCarbon)
    {
        d->lifetime = newLifetimeCarbon;
        emit lifetimeCarbonChanged();
    }
}

void CarbonProcessor::setCarbonUsageLevel(CarbonUsageLevel newLevel)
{
    Q_ASSERT(d != nullptr);

    INF(QString("New cabon usage level is: %1").arg(newLevel));

    if(d->usageLevel != newLevel)
    {
        d->usageLevel = newLevel;
        emit carbonUsageLevelChanged();
    }
}

void CarbonProcessor::setChargeForecast(ChargeForecast newChargeForecast)
{
    Q_ASSERT(d != nullptr);

    INF(QString("New charge forecast: %1.").arg(newChargeForecast));

    if(d->chargeForecast != newChargeForecast)
    {
        d->chargeForecast = newChargeForecast;
        emit chargeForecastChanged();
    }
}
/* static */
bool CarbonProcessor::isOutOfDate(const CarbonData &data)
{
    if(!data.isValid)
    {
        return true;
    }

    if(QDateTime::currentDateTime() <= data.validTo)
    {
        return false;
    }

    return true;
}
/* static */
CarbonProcessor::ChargeForecast CarbonProcessor::calculateChargeForecast(const CarbonData &data)
{
    if(!data.isValid)
    {
        return ChargeWhenNeeded;
    }

    int now = data.co2PerkWhNow;
    int next = data.co2PerkWhNext;
    int later = data.co2PerkWhLater;

    // Now is less than anytime in near future
    if(now <= next && now < later)
    {
        return ChargeNow;
    } else if(now > next)
    {
        return ChargeIn30;
    } else if(now > later)
    {
        return ChargeIn60;
    }

    return ChargeWhenNeeded;
}
