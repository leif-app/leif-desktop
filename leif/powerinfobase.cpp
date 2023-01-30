#include <QTimer>

#include "powerinfobase.h"
#include "log/log.h"

class PowerInfoBasePrivate
{
    PowerInfoBasePrivate();
    ~PowerInfoBasePrivate() = default;

    // DATA
    int checkIntervalInMinutes;
    int lastCapacity;
    int currentCapacity;
    int currentChargeRate;
    int currentDischargeRate;
    int avarageDischargeRate;
    PowerInfoBase::State state;

    friend class PowerInfoBase;
};

PowerInfoBasePrivate::PowerInfoBasePrivate():
    checkIntervalInMinutes {5},
    lastCapacity {0},
    currentCapacity {0},
    currentChargeRate {0},
    currentDischargeRate {0},
    avarageDischargeRate {0},
    state {PowerInfoBase::Unknown}
{}

PowerInfoBase::PowerInfoBase(QObject *parent /* = nullptr */):
    QObject(parent),
    d {new PowerInfoBasePrivate}
{
    QTimer *checkTimer = new QTimer(this);
    checkTimer->setInterval(d->checkIntervalInMinutes * 1000);
    checkTimer->setSingleShot(false);
    connect(checkTimer, &QTimer::timeout, this, &PowerInfoBase::checkLevels);
    checkTimer->start();
}

PowerInfoBase::~PowerInfoBase()
{
    delete d;
    d = nullptr;
}

PowerInfoBase::State PowerInfoBase::state() const
{
    Q_ASSERT(d != nullptr);

    return d->state;
}

float PowerInfoBase::powerDrawInWatts()
{
    DBG_CALLED;

    updateState();

    float consumption = 0;

    switch(state())
    {
    case(PowerInfoBase::Unknown):
        // Shouldn't happen. Some issue with access
        WRN("Unknown state detected.");
        break;

    case(PowerInfoBase::NoBattery):
        // We can't get any information. There is no battery in the system.
        INF("No battery. Estimating usage.");
        consumption = PowerInfoBase::noBatteryPowerEstimate();
        break;

    case(PowerInfoBase::FullyCharged):
        // There is no live data. We need to return the last discharge value
        // as it is the most accurate power consumption value we have.
        INF("Fully charged. Returning last discharge value.");
        consumption = avarageDischargeConsumption();
        break;

    case(PowerInfoBase::Charging):
        // We are charging. We will return the current charge rate
        INF("Charging. Returning charge rate.");
        consumption = chargeConsumption();
        break;

    case(PowerInfoBase::Discharging):
        // Since we are discharging, we are not using any power from the grid.
        INF("Discharging. No power draw.");
        break;
    }

    DBG(QString("Detected consumption: %1.").arg(consumption));
    return consumption;
}

void PowerInfoBase::checkLevels()
{
    DBG_CALLED;

    Q_ASSERT(d != nullptr);

    updateState();

    if(state() == PowerInfoBase::Discharging)
    {
        DBG("Discharging. Checking discharge rate.");
        if(d->currentDischargeRate == 0)
        {
            DBG("Discharge rate is zero.");
            if(d->lastCapacity > 0)
            {
                DBG(QString("Last capacity greater than zero (%1).").arg(d->lastCapacity));
                d->avarageDischargeRate = d->lastCapacity - d->currentCapacity;

                DBG(QString("AVGDischargeRate = %1 - %2").arg(d->lastCapacity).arg(d->currentChargeRate));
            }

            d->lastCapacity = d->currentCapacity;
        }
    }
}

/**
 * @brief Checks the current SYSTEM_BATTERY_STATE.
 */
void PowerInfoBase::updateState()
{
    DBG_CALLED;

    Q_ASSERT(d != nullptr);

    d->state = PowerInfoBase::Unknown;

    if(!hasBattery())
    {
        DBG("No battery detected.");
        d->state = PowerInfoBase::NoBattery;
        return;
    }

    /*
    if(batteryFullyCharged())
    {
        d->state = PowerInfoBase::FullyCharged;
        return;
    }
    */

    if(batteryCharging())
    {
        DBG("Battery is charging.");
        d->state = PowerInfoBase::Charging;
        d->currentChargeRate = chargeRate();

        DBG(QString("Current charge rate is: %1.").arg(d->currentChargeRate));
        return;
    }

    d->state = PowerInfoBase::Discharging;
    d->currentDischargeRate = dischargeRate();
    d->currentCapacity = currentCapacity();

    DBG(QString("Discharging. Current discharge rate: %1, current capacity: "
                "%2.").arg(d->currentDischargeRate).arg(d->currentCapacity));
}

float PowerInfoBase::avarageDischargeConsumption()
{
    DBG_CALLED;

    Q_ASSERT(d != nullptr);

    if(d->currentDischargeRate > 0 )
    {
        DBG(QString("Current discharge rate is: %1.").arg(d->currentDischargeRate));
        return d->currentDischargeRate;
    }

    DBG(QString("Returning avarage discharge rate: %1.").arg(d->avarageDischargeRate));
    return d->avarageDischargeRate;
}

float PowerInfoBase::chargeConsumption()
{
    DBG_CALLED;

    Q_ASSERT(d != nullptr);

    DBG(QString("Returning charge consumption: %1.").arg(d->currentChargeRate / 1000));
    return d->currentChargeRate / 1000;
}


float PowerInfoBase::noBatteryPowerEstimate()
{
    DBG_CALLED;
    DBG("Returning 200 as a no battery power estimate.");

    return 200;
}
