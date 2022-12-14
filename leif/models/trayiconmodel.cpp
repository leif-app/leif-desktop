#include "models/carbonmodel.h"
#include "models/trayiconmodel.h"
#include "models/settingsmodel.h"
#include "plugin/carbonpluginmanager.h"

class TrayIconModelPrivate
{
    TrayIconModelPrivate();
    ~TrayIconModelPrivate();

    TrayIconModel::ChargeForecast chargeForecast;
    QQmlApplicationEngine *qmlEngine;
    SettingsModel *settingsModel;
    CarbonModel *carbonModel;

    friend class TrayIconModel;
};

TrayIconModelPrivate::TrayIconModelPrivate():
    chargeForecast{TrayIconModel::ChargeWhenNeeded},
    qmlEngine{new QQmlApplicationEngine},
    settingsModel{new SettingsModel},
    carbonModel{new CarbonModel}
{}

TrayIconModelPrivate::~TrayIconModelPrivate()
{
    delete qmlEngine;
    qmlEngine = nullptr;

    delete settingsModel;
    settingsModel = nullptr;

    delete carbonModel;
    carbonModel = nullptr;
}

TrayIconModel::TrayIconModel(QObject *parent /* = nullptr */):
    QObject(parent),
    d{new TrayIconModelPrivate}
{
    connect(d->carbonModel, &CarbonModel::sessionCarbonChanged, this, &TrayIconModel::sessionCarbonChanged);
    connect(d->carbonModel, &CarbonModel::lifetimeCarbonChanged, this, &TrayIconModel::lifetimeCarbonChanged);
    connect(d->carbonModel, &CarbonModel::carbonUsageLevelChanged, this, &TrayIconModel::carbonUsageLevelChanged);
    connect(d->settingsModel, &SettingsModel::countryChanged, this, &TrayIconModel::configuredChanged);
    connect(d->settingsModel, &SettingsModel::regionIdChanged, this, &TrayIconModel::configuredChanged);

    if(configured())
    {
        CarbonPluginManager *manager = CarbonPluginManager::Instance();

        if(manager != nullptr)
        {
            manager->loadPlugin(d->settingsModel->country());
        }
    }
}

TrayIconModel::~TrayIconModel()
{
    delete d;
    d = nullptr;
}

int TrayIconModel::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonModel == nullptr)
    {
        return 0;
    }

    return d->carbonModel->sessionCarbon();
}

int TrayIconModel::lifetimeCarbon() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonModel == nullptr)
    {
        return 0;
    }

    return d->carbonModel->lifetimeCarbon();
}

CarbonProcessor::CarbonUsageLevel TrayIconModel::carbonUsageLevel() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonModel == nullptr)
    {
        return CarbonProcessor::VeryHigh;
    }

    return d->carbonModel->carbonUsageLevel();
}

TrayIconModel::ChargeForecast TrayIconModel::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    return d->chargeForecast;
}

void TrayIconModel::setChargeForecast(ChargeForecast newChargeForecast)
{
    Q_ASSERT(d != nullptr);

    if (d->chargeForecast == newChargeForecast)
        return;

    d->chargeForecast = newChargeForecast;
    emit chargeForecastChanged();
}

void TrayIconModel::resetStats()
{
    Q_ASSERT(d != nullptr);

    if(d->carbonModel == nullptr)
    {
        return;
    }

    d->carbonModel->clearStats();
}

void TrayIconModel::showDialog()
{
    Q_ASSERT(d != nullptr);

    if(d->qmlEngine != nullptr)
    {
        d->qmlEngine->load("qrc:///qml/main.qml");
    }
}

void TrayIconModel::onConfiguredChanged()
{
    Q_ASSERT(d != nullptr);

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager != nullptr)
    {
        manager->loadPlugin(d->settingsModel->country());
    }
}

bool TrayIconModel::configured() const
{
    Q_ASSERT(d != nullptr);

    if(d->settingsModel == nullptr)
    {
        return false;
    }

    if(d->settingsModel->country() == QLocale::AnyCountry)
    {
        return false;
    }

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        return false;
    }

    if(manager->regionIds(d->settingsModel->country()).isEmpty())
    {
        return true;
    }

    return !d->settingsModel->regionId().isEmpty();
}
