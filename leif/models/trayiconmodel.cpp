#include <QQuickWindow>

#include "models/carbonmodel.h"
#include "models/trayiconmodel.h"
#include "models/settingsmodel.h"
#include "plugin/carbonpluginmanager.h"
#include "utils/qmlwarninglogger.h"
#include "leifsettings.h"

class TrayIconModelPrivate
{
    TrayIconModelPrivate();
    ~TrayIconModelPrivate();

    QQmlApplicationEngine *qmlEngine;
    QQuickWindow *qmlWindow;
    SettingsModel *settingsModel;
    CarbonModel *carbonModel;

    friend class TrayIconModel;
};

TrayIconModelPrivate::TrayIconModelPrivate():
    qmlEngine{new QQmlApplicationEngine},
    qmlWindow{nullptr},
    settingsModel{new SettingsModel},
    carbonModel{new CarbonModel}
{}

TrayIconModelPrivate::~TrayIconModelPrivate()
{
    delete qmlWindow;
    qmlWindow = nullptr;

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
    connect(d->carbonModel, &CarbonModel::chargeForecastChanged, this, &TrayIconModel::chargeForecastChanged);
    connect(d->settingsModel, &SettingsModel::countryChanged, this, &TrayIconModel::configuredChanged);
    connect(d->settingsModel, &SettingsModel::regionIdChanged, this, &TrayIconModel::configuredChanged);
    connect(d->qmlEngine, &QQmlApplicationEngine::objectCreated, this, &TrayIconModel::onObjectCreated);

    if(configured())
    {
        CarbonPluginManager *manager = CarbonPluginManager::Instance();

        if(manager != nullptr)
        {
            manager->loadPlugin(d->settingsModel->country());
        }
    }

    // Install QmlWarningLogger
    new Utils::QmlWarningLogger(d->qmlEngine);
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

CarbonProcessor::ChargeForecast TrayIconModel::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonModel == nullptr)
    {
        return CarbonProcessor::ChargeWhenNeeded;
    }

    return d->carbonModel->chargeForecast();
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

    if(d->qmlWindow != nullptr)
    {
        d->qmlWindow->show();
        d->qmlWindow->raise();
    }
    else if(d->qmlEngine != nullptr)
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

void TrayIconModel::onObjectCreated(QObject *object, const QUrl &url)
{
    Q_ASSERT(d != nullptr);

    Q_UNUSED(url);

    QQuickWindow *qmlWindow = qobject_cast<QQuickWindow*>(object);
    if(qmlWindow != nullptr)
    {
        d->qmlWindow = qmlWindow;
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
