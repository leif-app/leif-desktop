#ifndef POWERINFO_H
#define POWERINFO_H

#include "powerinfobase.h"

class PowerInfo : public PowerInfoBase
{
public:
    explicit PowerInfo(QObject *parent = nullptr);
    virtual ~PowerInfo() = default;

    // PowerInfoBase interface
protected:
    virtual bool hasBattery() override;
    virtual bool batteryFullyCharged() override;
    virtual bool batteryCharging() override;
    virtual int chargeRate() override;
    virtual int dischargeRate() override;
    virtual int currentCapacity() override;
};

#endif // POWERINFO_H
