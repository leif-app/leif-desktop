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
#ifndef TRAYICONMODEL_H
#define TRAYICONMODEL_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "carbonprocessor.h"

class TrayIconModelPrivate;

class TrayIconModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sessionCarbon READ sessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(int lifetimeCarbon READ lifetimeCarbon NOTIFY lifetimeCarbonChanged)
    Q_PROPERTY(CarbonProcessor::CarbonUsageLevel carbonUsageLevel READ carbonUsageLevel NOTIFY carbonUsageLevelChanged)
    Q_PROPERTY(ChargeForecast chargeForecast READ chargeForecast WRITE setChargeForecast NOTIFY chargeForecastChanged)
    Q_PROPERTY(bool configured READ configured NOTIFY configuredChanged)

public:
    enum ChargeForecast { ChargeNow, ChargeIn30, ChargeIn60, ChargeWhenNeeded };
    Q_ENUM(ChargeForecast)

    explicit TrayIconModel(QObject *parent = nullptr);
    virtual ~TrayIconModel();

    int sessionCarbon() const;
    int lifetimeCarbon() const;

    CarbonProcessor::CarbonUsageLevel carbonUsageLevel() const;

    ChargeForecast chargeForecast() const;
    void setChargeForecast(ChargeForecast newChargeForecast);

    bool configured() const;

public Q_SLOTS:
    void resetStats();
    void showDialog();

private Q_SLOTS:
    void onConfiguredChanged();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();
    void carbonUsageLevelChanged();
    void chargeForecastChanged();
    void configuredChanged();

private:
    TrayIconModelPrivate *d;
};

#endif // TRAYICONMODEL_H
