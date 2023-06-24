#include <QQuickWindow>

#include <services/settingsservice.h>
#include <services/carbonservice.h>

#include <controllers/carboncontroller.h>
#include <controllers/settingscontroller.h>
#include <controllers/trayiconcontroller.h>
#include <plugin/carbonpluginmanager.h>
#include <utils/qmlwarninglogger.h>

class TrayIconControllerPrivate
{
public:
    ~TrayIconControllerPrivate() = default;

private:
    TrayIconControllerPrivate(SettingsService *settingsService, CarbonService *carbonService);

    template <class T>
    void registerQmlController(const char *name, T *controller)
    {
        QObject::connect(qmlEngine.get(), &QQmlApplicationEngine::destroyed, controller, [=](){ controller->deleteLater();});
        qmlRegisterSingletonInstance<T>("Leif.Controllers", 1, 0, name, controller);
    }

    QScopedPointer<QQmlApplicationEngine> qmlEngine;
    QScopedPointer<QQuickWindow> qmlWindow;
    SettingsService* settingsService;
    CarbonService *carbonService;
    
    friend class TrayIconController;
};

TrayIconControllerPrivate::TrayIconControllerPrivate(SettingsService *settingsService,
                                                     CarbonService *carbonService):
    qmlEngine{new QQmlApplicationEngine},
    qmlWindow{nullptr},
    settingsService{settingsService},
    carbonService{carbonService}
{
    SettingsController *settingsController {new SettingsController {settingsService}};
    CarbonController *carbonController {new CarbonController {carbonService}};

    registerQmlController<SettingsController>("SettingsController", settingsController);
    registerQmlController<CarbonController>("CarbonController", carbonController);
}

TrayIconController::TrayIconController(SettingsService *settingsService,
                                       CarbonService *carbonService,
                                       QObject *parent /* = nullptr */):
    QObject(parent),
    d{new TrayIconControllerPrivate {settingsService, carbonService}}
{
    connect(d->carbonService, &CarbonService::sessionCarbonChanged, this, [=]() {emit sessionCarbonChanged(sessionCarbon());});
    connect(d->carbonService, &CarbonService::lifetimeCarbonChanged, this, [=]() {emit lifetimeCarbonChanged(lifetimeCarbon());});
    connect(d->carbonService, &CarbonService::carbonUsageLevelChanged, this, [=]() {emit carbonUsageLevelChanged(carbonUsageLevel());});
    connect(d->carbonService, &CarbonService::chargeForecastChanged, this, [=]() {emit chargeForecastChanged(chargeForecast());});
    connect(d->settingsService, &SettingsService::countryChanged, this, &TrayIconController::configuredChanged);
    connect(d->settingsService, &SettingsService::regionIdChanged, this, &TrayIconController::configuredChanged);
    connect(d->qmlEngine.get(), &QQmlApplicationEngine::objectCreated, this, &TrayIconController::onObjectCreated);

    if(configured())
    {
        CarbonPluginManager *manager = CarbonPluginManager::Instance();

        if(manager != nullptr)
            manager->loadPlugin(d->settingsService->country());
    }

    // Install QmlWarningLogger
    new Utils::QmlWarningLogger(d->qmlEngine.get());
}

TrayIconController::~TrayIconController()
{}

float TrayIconController::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonService == nullptr)
        return 0.0;

    return d->carbonService->sessionCarbon();
}

float TrayIconController::lifetimeCarbon() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonService == nullptr)
        return 0.0;

    return d->carbonService->lifetimeCarbon();
}

CarbonUsageLevel TrayIconController::carbonUsageLevel() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonService == nullptr)
        return CarbonUsageLevel::VeryHigh;

    return d->carbonService->carbonUsageLevel();
}

ChargeForecast TrayIconController::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    if(d->carbonService == nullptr)
        return ChargeForecast::ChargeWhenNeeded;

    return d->carbonService->chargeForecast();
}

void TrayIconController::resetStats()
{
    Q_ASSERT(d != nullptr);

    if(d->carbonService == nullptr)
        return;

    d->carbonService->clearStats();
}

void TrayIconController::showDialog()
{
    Q_ASSERT(d != nullptr);

    if(d->qmlWindow != nullptr)
    {
        d->qmlWindow->show();
        d->qmlWindow->raise();
    }
    else if(d->qmlEngine != nullptr)
        d->qmlEngine->load("qrc:///qml/main.qml");
}

void TrayIconController::onConfiguredChanged()
{
    Q_ASSERT(d != nullptr);

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager != nullptr)
        manager->loadPlugin(d->settingsService->country());
}

void TrayIconController::onObjectCreated(QObject *object, const QUrl &url)
{
    Q_ASSERT(d != nullptr);

    Q_UNUSED(url);

    QQuickWindow *qmlWindow = qobject_cast<QQuickWindow*>(object);
    if(qmlWindow != nullptr)
        d->qmlWindow.reset(qmlWindow);
}

bool TrayIconController::configured() const
{
    Q_ASSERT(d != nullptr);

    if(d->settingsService == nullptr)
        return false;

    if(d->settingsService->country() == QLocale::AnyCountry)
        return false;

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
        return false;

    if(manager->regionIds(d->settingsService->country()).isEmpty())
        return true;

    return !d->settingsService->regionId().isEmpty();
}
