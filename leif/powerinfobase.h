#ifndef POWERINFOBASE_H
#define POWERINFOBASE_H

#include <QObject>
#include <interfaces/IPower.h>

class PowerInfoBasePrivate;

class PowerInfoBase : public QObject, public IPower
{
Q_OBJECT

public:
    enum State {Unknown, NoBattery, FullyCharged, Charging, Discharging};
    Q_ENUM(State)
    Q_INTERFACES(IPower)

    explicit PowerInfoBase(QObject *parent = nullptr);
    virtual ~PowerInfoBase();

    State state() const;
    virtual float powerDrawInWatts() override;

protected:
    virtual bool hasBattery() = 0;
    virtual bool batteryFullyCharged() = 0;
    virtual bool batteryCharging() = 0;
    virtual int chargeRate() = 0;
    virtual int dischargeRate() = 0;
    virtual int currentCapacity() = 0;

private slots:
    void checkLevels();

private:
    void updateState();
    float avarageDischargeConsumption();
    float chargeConsumption();

    static float noBatteryPowerEstimate();

private:
    Q_DISABLE_COPY_MOVE(PowerInfoBase)
    PowerInfoBasePrivate *d;
};
#endif // POWERINFOBASE_H
