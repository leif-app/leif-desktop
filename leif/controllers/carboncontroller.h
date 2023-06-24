/**
 * \brief Defines the CarbonModel class.
 *
 * The CarbonModel class is a QML usable wrapper around the CarbonService among others.
 * It informs about changes in the lifetime carbon counter and the session carbon counter.
 * Also it informs about the current carbon usage "intensity".
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 *
 * \date 17.09.2022
 */
#ifndef CARBONCONTROLLER_H
#define CARBONCONTROLLER_H

#include <QObject>
#include <QQmlEngine>

#include <include/carbonusagelevel.h>
#include <include/chargeforecast.h>

class CarbonService;

class CarbonController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float sessionCarbon READ sessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(float lifetimeCarbon READ lifetimeCarbon NOTIFY lifetimeCarbonChanged)
    Q_PROPERTY(CarbonUsageLevel carbonUsageLevel READ carbonUsageLevel NOTIFY carbonUsageLevelChanged)
    Q_PROPERTY(ChargeForecast chargeForecast READ chargeForecast NOTIFY chargeForecastChanged)

public:
    explicit CarbonController(CarbonService* carbonService, QObject *parent = nullptr);

    float sessionCarbon() const;
    float lifetimeCarbon() const;
    CarbonUsageLevel carbonUsageLevel() const;
    ChargeForecast chargeForecast() const;

public slots:
    void clearStats();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();
    void carbonUsageLevelChanged();
    void chargeForecastChanged();

private:
    struct CarbonModelPrivate
    {
        CarbonService *carbonService;
    } d;
};

#endif // CARBONCONTROLLER_H
