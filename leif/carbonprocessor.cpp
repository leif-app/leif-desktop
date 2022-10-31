#include <QTimer>

#include "carbonprocessor.h"
#include "leifsettings.h"
#include "powerfactory.h"

#include "plugin/carbonpluginmanager.h"

#include "interfaces/IPower.h"

class CarbonProcessorPrivate
{
    static CarbonProcessor *Instance;

    int session;
    int lifetime;
    IPower *powerInfo;

    QTimer calculateTimer;

    friend class CarbonProcessor;
};

CarbonProcessor * CarbonProcessorPrivate::Instance = nullptr;

CarbonProcessor::CarbonProcessor(QObject *parent)
    : QObject{parent},
      d{new CarbonProcessorPrivate}
{
    d->session = 0;
    d->lifetime = 0;
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

int CarbonProcessor::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->session;
}

int CarbonProcessor::lifetimeCarbon() const
{
    Q_ASSERT(d != nullptr);

    return d->lifetime;
}

void CarbonProcessor::clearStats()
{
    setSessionCarbon(0);
    setLifetimeCarbon(0);
}

void CarbonProcessor::calculateCarbon()
{
    Q_ASSERT(d != nullptr);

    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        return;
    }

    float powerDraw = d->powerInfo->powerDrawInWatts();
    CarbonData data = manager->carbonPerKiloWatt(settings->country(), settings->regionId());

    if(data.isValid)
    {
        float carbon = (powerDraw * static_cast<float>(data.co2PerKiloWattHour)) / 60;
        setSessionCarbon(sessionCarbon() + carbon);
        setLifetimeCarbon(lifetimeCarbon() + carbon);
    }
}

void CarbonProcessor::setSessionCarbon(int newSessionCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->session != newSessionCarbon)
    {
        d->session = newSessionCarbon;
        emit sessionCarbonChanged();
    }
}

void CarbonProcessor::setLifetimeCarbon(int newLifetimeCarbon)
{
    Q_ASSERT(d != nullptr);

    if(d->lifetime != newLifetimeCarbon)
    {
        d->lifetime = newLifetimeCarbon;
        emit lifetimeCarbonChanged();
    }
}
