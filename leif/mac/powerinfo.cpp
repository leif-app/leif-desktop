#include <CoreFoundation/CFNumber.h>

#include "powerinfo.h"

PowerInfo::PowerInfo(QObject *parent /* = nullptr */):
    PowerInfoBase(parent)
{
    m_service = IOServiceGetMatchingService(kIOMasterPortDefault,
                                            IOServiceNameMatching("AppleSmartBattery"));
}

PowerInfo::~PowerInfo()
{
    if(m_service != 0)
    {
        IOObjectRelease(m_service);
        m_service = 0;
    }
}

bool PowerInfo::hasBattery()
{
    if(m_service == 0)
    {
        return false;
    }

    bool isBatteryInstalled = false;
    QString batteryInstalledKey = "BatteryInstalled"

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, batteryInstalledKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        bool isBatteryInstalled = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }

    return isBatteryInstalled;
}

bool PowerInfo::batteryFullyCharged()
{
    if(m_service == 0)
    {
        return false;
    }

    bool isFullyCharged = false;
    QString fullyChargedKey = "FullyCharged"

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, fullyChargedKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        isFullyCharged = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }

    return isFullyCharged;
}

bool PowerInfo::batteryCharging()
{
    if(m_service == 0)
    {
        return false;
    }

    bool isCharging = false;
    QString isChargingKey = "IsCharging";

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, isChargingKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        isChargingKey = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }

    return isCharging;
}

int PowerInfo::chargeRate()
{
    if(m_service == 0)
    {
        return false;
    }

    int adapterPower = 0;
    QString batteryDataKey = "BatteryData";
    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, batteryDataKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        CFDictionaryRef dictRef = (CFDictionaryRef)(prop);
        QString key = "AdapterPower";
        const void *data = CFDictionaryGetValue(dictRef, key.toCFString());

        CFNumberGetValue((CFNumberRef)data, CFNumberGetType((CFNumberRef)data), &adapterPower);

        QString powerStr = QString::number(adapterPower);
        bool ok = false;
        adapterPower = powerStr.toUInt(&ok, 16);

        CFRelease(prop);
        prop = nullptr;
    }

    return adapterPower;
}

int PowerInfo::dischargeRate()
{
    return 0;
}

int PowerInfo::currentCapacity()
{
    if(m_service == 0)
    {
        return false;
    }

    int theCurrentCapacity = 0;
    QString currentCapacityKey = "CurrentCapacity";

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, currentCapacityKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        CFNumberGetValue((CFNumberRef)prop, CFNumberGetType((CFNumberRef)prop), &theCurrentCapacity);

        QString str = QString::number(theCurrentCapacity);
        bool ok = false;
        theCurrentCapacity = str.toUInt(&ok, 16);

        CFRelease(prop);
        prop = nullptr;
    }

    return theCurrentCapacity;
}
