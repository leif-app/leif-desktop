#include <QTimer>

#include "powerinfobase.h"

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
    updateState();

    float consumption = 0;

    switch(state())
    {
    case(PowerInfoBase::Unknown):
        // Shouldn't happen. Some issue with access
        break;

    case(PowerInfoBase::NoBattery):
        // We can't get any information. There is no battery in the system.
        consumption = PowerInfoBase::noBatteryPowerEstimate();
        break;

    case(PowerInfoBase::FullyCharged):
        // There is no live data. We need to return the last discharge value
        // as it is the most accurate power consumption value we have.
        consumption = avarageDischargeConsumption();
        break;

    case(PowerInfoBase::Charging):
        // We are charging. We will return the current charge rate
        consumption = chargeConsumption();
        break;

    case(PowerInfoBase::Discharging):
        // Since we are discharging, we are not using any power from the grid.
        break;
    }

    return consumption;
}

void PowerInfoBase::checkLevels()
{
    Q_ASSERT(d != nullptr);

    updateState();

    if(state() == PowerInfoBase::Discharging)
    {
        if(d->currentDischargeRate == 0)
        {
            if(d->lastCapacity > 0)
            {
                d->avarageDischargeRate = d->lastCapacity - d->currentCapacity;
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
    Q_ASSERT(d != nullptr);

    d->state = PowerInfoBase::Unknown;

    if(!hasBattery())
    {
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
        d->state = PowerInfoBase::Charging;
        d->currentChargeRate = chargeRate();
        return;
    }

    d->state = PowerInfoBase::Discharging;
    d->currentDischargeRate = dischargeRate();
    d->currentCapacity = currentCapacity();
}

float PowerInfoBase::avarageDischargeConsumption()
{
    Q_ASSERT(d != nullptr);

    if(d->currentDischargeRate > 0 )
    {
        return d->currentDischargeRate;
    }

    return d->avarageDischargeRate;
}

float PowerInfoBase::chargeConsumption()
{
    Q_ASSERT(d != nullptr);

    return d->currentChargeRate / 1000;
}


float PowerInfoBase::noBatteryPowerEstimate()
{
    return 200;
}
