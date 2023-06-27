#include <QApplication>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QSysInfo>
#include <QSysInfo>

#include <controllers/trayiconcontroller.h>

#include "trayicon.h"

class TrayIconPrivate
{
public:
    ~TrayIconPrivate() = default;

private:
    TrayIconPrivate(TrayIconController *_controller);

    static QString sessionCarbonLabel(double value);
    static QString totalCarbonLabel(double value);
    static QString carbonLabel(const QString &labelText, double value);
    static QString intensityLabel(CarbonUsageLevel usageLevel);
    static QString chargeForecastLabel(ChargeForecast chargeForecast);

    static QString co2Unit();
    static QString usageLevelLabel(CarbonUsageLevel usageLevel);
    static QString usageLevelIconName(CarbonUsageLevel usageLevel);
    static QString contrastModeImagePath(bool contrastMode);
    static QString iconName(CarbonUsageLevel usageLevel, bool contrastMode);
    static bool currentContrastMode();

    TrayIconController *controller;
    QAction *notConfiguredAction;
    QAction *sessionCarbonAction;
    QAction *totalCarbonAction;
    QAction *carbonUsageLevelAction;
    QAction *chargeForecastAction;

    friend class TrayIcon;
};

TrayIconPrivate::TrayIconPrivate(TrayIconController *_controller):
    controller {_controller},
    notConfiguredAction {nullptr},
    sessionCarbonAction {nullptr},
    totalCarbonAction {nullptr},
    carbonUsageLevelAction {nullptr},
    chargeForecastAction {nullptr}
{}

QString TrayIconPrivate::co2Unit()
{
    return QString("gCO₂e");
}

QString TrayIconPrivate::sessionCarbonLabel(double value)
{
    QString labelText = QCoreApplication::translate("TrayIcon", "Session", "As in, since the computer was turned on.");
    return TrayIconPrivate::carbonLabel(labelText, value);
}

QString TrayIconPrivate::totalCarbonLabel(double value)
{
    QString labelText = QCoreApplication::translate("TrayIcon", "Lifetime", "As in, total sum of consumption");
    return TrayIconPrivate::carbonLabel(labelText, value);
}

QString TrayIconPrivate::carbonLabel(const QString &labelText, double value)
{
    QString label = QStringLiteral("%1: %2 %3");
    label = label.arg(labelText, QLocale().toString(value, 'g', 2), TrayIconPrivate::co2Unit());

    return label;
}

QString TrayIconPrivate::intensityLabel(CarbonUsageLevel usageLevel)
{
    QString intensity = QStringLiteral("%1: %2");
    intensity = intensity.arg(QCoreApplication::translate("TrayIcon", "Current intensity", "Power usage intensity."), TrayIconPrivate::usageLevelLabel(usageLevel));

    return intensity;
}

QString TrayIconPrivate::chargeForecastLabel(ChargeForecast chargeForecast)
{
    static QHash<ChargeForecast, QString> label;

    if(label.contains(chargeForecast))
        return label.value(chargeForecast);

    QString str;

    switch(chargeForecast)
    {
    case(ChargeForecast::ChargeNow):
        str = QCoreApplication::translate("TrayIcon", "Charge now");
        break;

    case(ChargeForecast::ChargeIn30):
        str = QCoreApplication::translate("TrayIcon", "Charge in 30 minutes");
        break;

    case(ChargeForecast::ChargeIn60):
        str = QCoreApplication::translate("TrayIcon", "Charge in an hour or so");
        break;

    case(ChargeForecast::ChargeWhenNeeded):
        str = QCoreApplication::translate("TrayIcon", "Charge when needed");
        break;
    }

    label.insert(chargeForecast, str);

    return str;
}

QString TrayIconPrivate::usageLevelLabel(CarbonUsageLevel usageLevel)
{
    static QHash<CarbonUsageLevel, QString> usageLevelLabels;

    if(usageLevelLabels.contains(usageLevel))
        return usageLevelLabels.value(usageLevel);

    QString str;
    switch(usageLevel)
    {
    case(CarbonUsageLevel::VeryHigh):
        //: The carbon usage intensity is very high
        str = QCoreApplication::translate("TrayIcon", "Very high");
        break;

    case(CarbonUsageLevel::High):
        str = QCoreApplication::translate("TrayIcon", "High");
        break;

    case(CarbonUsageLevel::Medium):
        str = QCoreApplication::translate("TrayIcon", "Medium");
        break;

    case(CarbonUsageLevel::Low):
        str = QCoreApplication::translate("TrayIcon", "Low");
        break;

    case(CarbonUsageLevel::VeryLow):
        str = QCoreApplication::translate("TrayIcon", "Very low");
        break;

    default:
        str = QCoreApplication::translate("TrayIcon", "unknown");
        break;
    }

    usageLevelLabels.insert(usageLevel, str);

    return str;
}

QString TrayIconPrivate::usageLevelIconName(CarbonUsageLevel usageLevel)
{
    static QHash<CarbonUsageLevel, QString> iconNames;

    if(iconNames.contains(usageLevel))
        return iconNames.value(usageLevel);

    QString str;
    switch(usageLevel)
    {
    case(CarbonUsageLevel::VeryHigh):
        str = QStringLiteral("verysad");
        break;

    case(CarbonUsageLevel::High):
        str = QStringLiteral("sad");
        break;

    case(CarbonUsageLevel::Medium):
        str = QStringLiteral("fair");
        break;

    case(CarbonUsageLevel::Low):
        str = QStringLiteral("happy");
        break;

    case(CarbonUsageLevel::VeryLow):
        str = QStringLiteral("veryhappy");
        break;

    default:
        str = QStringLiteral("verysad");
        break;
    }

    iconNames.insert(usageLevel, str);

    return str;
}

QString TrayIconPrivate::contrastModeImagePath(bool contrastMode)
{
    if(contrastMode)
        return QStringLiteral("dark");

    return QStringLiteral("light");
}

QString TrayIconPrivate::iconName(CarbonUsageLevel usageLevel, bool contrastMode)
{
    QString theIconName = QStringLiteral(":/img/tray/%1/%2.svg");
    theIconName = theIconName.arg(contrastModeImagePath(contrastMode),
                                  usageLevelIconName(usageLevel));

    return theIconName;
}

bool TrayIconPrivate::currentContrastMode()
{
#ifdef Q_OS_WINDOWS
    QString productVersion {QSysInfo::productVersion()};
    bool ok {false};
    unsigned int version {productVersion.toUInt(&ok)};

    if(!ok || version < 11)
        return true;
#endif // Q_OS_WINDOWS

    return false;
}


TrayIcon::TrayIcon(TrayIconController *model, QObject *parent /* = nullptr */):
    TrayIcon(model, QIcon(TrayIconPrivate::iconName(CarbonUsageLevel::VeryHigh, TrayIconPrivate::currentContrastMode())), parent)
{
    setupMenu();
    connectModel();

    QTimer::singleShot(500, this, &TrayIcon::doCheckConfigured);
}

TrayIcon::TrayIcon(TrayIconController *controller, const QIcon &icon, QObject *parent /* = nullptr */):
    QSystemTrayIcon(icon, parent),
    d {new TrayIconPrivate {controller}}
{}

TrayIcon::~TrayIcon()
{}

void TrayIcon::onCarbonUsageLevelChanged(CarbonUsageLevel newCarbonUsageLevel)
{
    Q_ASSERT(d != nullptr);

    d->carbonUsageLevelAction->setText(TrayIconPrivate::intensityLabel(newCarbonUsageLevel));

    // Also set the icon
    setIcon(QIcon(TrayIconPrivate::iconName(newCarbonUsageLevel, TrayIconPrivate::currentContrastMode())));
}

void TrayIcon::onResetStatsClicked()
{
    Q_ASSERT(d != nullptr);

    int answer = QMessageBox::question(nullptr, tr("Resetting stats"),
                                       tr("Do you really want to reset the "
                                          "carbon usage statistics?\n"
                                          "This cannot be undone."));

    if(answer != QMessageBox::Yes)
        return;

    showMessage(tr("Leif Information"), tr("The CO2 usage statistics have been cleared."),
                QIcon("img/leif_128.png"));

    d->controller->resetStats();
}

void TrayIcon::doCheckConfigured()
{
    Q_ASSERT(d != nullptr);

    if(!d->controller->configured())
        showMessage(tr("We need to talk"), tr("Hey there. I would love to go to "
                                              "work, but I need to know where "
                                              "you are."), QIcon("img/leif_128.png"));
}

void TrayIcon::setupMenu()
{
    QMenu *menu = new QMenu;

    d->notConfiguredAction = menu->addAction(tr("Leif is not configured yet"), this, [=](){d->controller->showDialog();});
    d->notConfiguredAction->setVisible(!d->controller->configured());

    d->sessionCarbonAction = menu->addAction(QString());
    d->sessionCarbonAction->setDisabled(true);
    d->sessionCarbonAction->setText(TrayIconPrivate::sessionCarbonLabel(d->controller->sessionCarbon()));

    d->totalCarbonAction = menu->addAction(QString());
    d->totalCarbonAction->setDisabled(true);
    d->totalCarbonAction->setText(TrayIconPrivate::totalCarbonLabel(d->controller->lifetimeCarbon()));

    d->carbonUsageLevelAction = menu->addAction(QString());
    d->carbonUsageLevelAction->setDisabled(true);
    onCarbonUsageLevelChanged(d->controller->carbonUsageLevel());

    d->chargeForecastAction = menu->addAction(QString());
    d->chargeForecastAction->setDisabled(true);
    d->chargeForecastAction->setText(TrayIconPrivate::chargeForecastLabel(d->controller->chargeForecast()));

    menu->addAction(tr("Preferences..."), this, [=](){d->controller->showDialog();});
    menu->addAction(tr("Reset stats"), Qt::Key_R, this, &TrayIcon::onResetStatsClicked);

    menu->addSeparator();
    menu->addAction(tr("Quit"), Qt::Key_Q, qApp, &QApplication::quit);

    setContextMenu(menu);
}

void TrayIcon::connectModel()
{
    Q_ASSERT(d != nullptr);
    
    connect(d->controller, &TrayIconController::sessionCarbonChanged, this, [=](float sessionCarbon){d->sessionCarbonAction->setText(TrayIconPrivate::sessionCarbonLabel(sessionCarbon));});
    connect(d->controller, &TrayIconController::lifetimeCarbonChanged, this, [=](float totalCarbon){d->totalCarbonAction->setText(TrayIconPrivate::totalCarbonLabel(totalCarbon));});
    connect(d->controller, &TrayIconController::carbonUsageLevelChanged, this, &TrayIcon::onCarbonUsageLevelChanged);
    connect(d->controller, &TrayIconController::chargeForecastChanged, this, [=](ChargeForecast newChargeForecast){d->chargeForecastAction->setText(TrayIconPrivate::chargeForecastLabel(newChargeForecast));});
    connect(d->controller, &TrayIconController::configuredChanged, this, [=](){d->notConfiguredAction->setVisible(!d->controller->configured());});
}
