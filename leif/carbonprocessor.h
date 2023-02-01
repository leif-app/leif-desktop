#ifndef CARBONPROCESSOR_H
#define CARBONPROCESSOR_H

#include <QObject>
#include "carbondata.h"

class CarbonProcessorPrivate;

class CarbonProcessor : public QObject
{
    Q_OBJECT
    explicit CarbonProcessor(QObject *parent = nullptr);
    virtual ~CarbonProcessor();

public:
    static CarbonProcessor *Instance();
    static void Destroy();

    enum CarbonUsageLevel { VeryLow, Low, Medium, High, VeryHigh };
    Q_ENUM(CarbonUsageLevel)

    enum ChargeForecast { ChargeNow, ChargeIn30, ChargeIn60, ChargeWhenNeeded };
    Q_ENUM(ChargeForecast)

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
    CarbonProcessor::CarbonUsageLevel calculateUsageLevel(int co2PerkWh);

private:
    void setSessionCarbon(float newSessionCarbon);
    void setLifetimeCarbon(float newLifetimeCarbon);
    void setCarbonUsageLevel(CarbonUsageLevel newLevel);
    void setChargeForecast(ChargeForecast newChargeForecast);
    static bool isOutOfDate(const CarbonData &data);
    static CarbonProcessor::ChargeForecast calculateChargeForecast(const CarbonData &data);

private:
    Q_DISABLE_COPY_MOVE(CarbonProcessor);
    CarbonProcessorPrivate *d;
};

#endif // CARBONPROCESSOR_H
