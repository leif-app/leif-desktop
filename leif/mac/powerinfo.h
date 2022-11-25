#ifndef POWERINFO_H
#define POWERINFO_H

#include <IOKit/IOKitLib.h>

#include "powerinfobase.h"

class PowerInfo : public PowerInfoBase
{
public:
    explicit PowerInfo(QObject *parent = nullptr);
    virtual ~PowerInfo();

    // PowerInfoBase interface
protected:
    virtual bool hasBattery() override;
    virtual bool batteryFullyCharged() override;
    virtual bool batteryCharging() override;
    virtual int chargeRate() override;
    virtual int dischargeRate() override;
    virtual int currentCapacity() override;

private:
    io_service_t m_service;
};

#endif // POWERINFO_H
