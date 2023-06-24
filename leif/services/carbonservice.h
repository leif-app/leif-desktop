#ifndef CARBONSERVICE_H
#define CARBONSERVICE_H

#include <QObject>

#include <include/carbonusagelevel.h>
#include <include/carbondata.h>
#include <include/chargeforecast.h>

class CarbonServicePrivate;
class SettingsService;

class CarbonService : public QObject
{
    Q_OBJECT
public:
    explicit CarbonService(SettingsService *settings, QObject *parent = nullptr);
    virtual ~CarbonService();

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

private slots:
    void calculateCarbon();
    CarbonUsageLevel calculateUsageLevel(int co2PerkWh);

private:
    void setSessionCarbon(float newSessionCarbon);
    void setLifetimeCarbon(float newLifetimeCarbon);
    void setCarbonUsageLevel(CarbonUsageLevel newLevel);
    void setChargeForecast(ChargeForecast newChargeForecast);
    static bool isOutOfDate(const CarbonData &data);
    static ChargeForecast calculateChargeForecast(const CarbonData &data);

private:
    Q_DISABLE_COPY_MOVE(CarbonService);
    QScopedPointer<CarbonServicePrivate> d;
};

#endif // CARBONSERVICE_H
