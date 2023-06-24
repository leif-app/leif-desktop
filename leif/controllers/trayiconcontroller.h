/**
 * @brief Defines the TrayIconModel class.
 *
 * The TrayIconModel class drives the data display of the TrayIcon instance. It
 * contains information about the current power usage, co2 footprint and more.
 *
 * @author Dariusz Scharsig
 *
 * @date 18.09.2022
 */
#ifndef TRAYICONCONTROLLER_H
#define TRAYICONCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include <include/chargeforecast.h>
#include <include/carbonusagelevel.h>

class CarbonService;
class SettingsService;
class TrayIconControllerPrivate;

class TrayIconController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float sessionCarbon READ sessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(float lifetimeCarbon READ lifetimeCarbon NOTIFY lifetimeCarbonChanged)
    Q_PROPERTY(CarbonUsageLevel carbonUsageLevel READ carbonUsageLevel NOTIFY carbonUsageLevelChanged)
    Q_PROPERTY(ChargeForecast chargeForecast READ chargeForecast NOTIFY chargeForecastChanged)
    Q_PROPERTY(bool configured READ configured NOTIFY configuredChanged)

public:
    explicit TrayIconController(SettingsService *settingsService,
                                CarbonService *carbonService,
                                QObject *parent = nullptr);

    virtual ~TrayIconController();

    float sessionCarbon() const;
    float lifetimeCarbon() const;
    CarbonUsageLevel carbonUsageLevel() const;
    ChargeForecast chargeForecast() const;

    bool configured() const;

public Q_SLOTS:
    void resetStats();
    void showDialog();

private Q_SLOTS:
    void onConfiguredChanged();
    void onObjectCreated(QObject *object, const QUrl &url);

signals:
    void sessionCarbonChanged(float newSessionCarbon);
    void lifetimeCarbonChanged(float newLifetimeCarbon);
    void carbonUsageLevelChanged(CarbonUsageLevel newCarbonUsageLevel);
    void chargeForecastChanged(ChargeForecast newChargeForecast);
    void configuredChanged();

private:
    QScopedPointer<TrayIconControllerPrivate> d;
};

#endif // TRAYICONCONTROLLER_H
