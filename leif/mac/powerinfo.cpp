#include <CoreFoundation/CFNumber.h>

#include "log/log.h"

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
    DBG_CALLED;

    if(m_service == 0)
    {
        WRN("No information service created.");
        return false;
    }

    bool isBatteryInstalled = false;
    QString batteryInstalledKey = "BatteryInstalled";

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, batteryInstalledKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {   
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        isBatteryInstalled = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }
    else
    {
        WRN(QString("Property with name '%1' not found.").arg(batteryInstalledKey));
    }

    if(isBatteryInstalled) DBG("Battery is installed.");
    if(!isBatteryInstalled) WRN("No battery found!");

    return isBatteryInstalled;
}

bool PowerInfo::batteryFullyCharged()
{
    DBG_CALLED;

    if(m_service == 0)
    {
        WRN("No information service created.");
        return false;
    }

    bool isFullyCharged = false;
    QString fullyChargedKey = "FullyCharged";

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, fullyChargedKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        isFullyCharged = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }
    else
    {
        WRN(QString("Property with name '%1' not found.").arg(fullyChargedKey));
    }

    if(isFullyCharged) DBG("Battery is fully charged.");
    if(!isFullyCharged) DBG("Battery is not fully charged yet.");
    return isFullyCharged;
}

bool PowerInfo::batteryCharging()
{
    DBG_CALLED;

    if(m_service == 0)
    {
        WRN("No information service created.");
        return false;
    }

    bool isCharging = false;
    QString isChargingKey = "IsCharging";

    CFTypeRef prop = IORegistryEntryCreateCFProperty(m_service, isChargingKey.toCFString(), NULL, 0);

    if(prop != nullptr)
    {
        Boolean bValue = CFBooleanGetValue((CFBooleanRef)prop);

        isCharging = bValue != 0;

        CFRelease(prop);
        prop = nullptr;
    }
    else
    {
        WRN(QString("Property with name '%1' not found.").arg(isChargingKey));
    }

    if(isCharging) DBG("Battery is charging.");
    if(!isCharging) DBG("Battery is not charging.");

    return isCharging;
}

int PowerInfo::chargeRate()
{
    DBG_CALLED;

    if(m_service == 0)
    {
        WRN("No information service created.");
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

        if(data == nullptr)
        {
            WRN(QString("DictRef Property with name '%1' not found.").arg(key));
        }

        CFNumberGetValue((CFNumberRef)data, CFNumberGetType((CFNumberRef)data), &adapterPower);

        QString powerStr = QString::number(adapterPower);
        bool ok = false;
        adapterPower = powerStr.toUInt(&ok, 16);

        if(!ok)
        {
            WRN(QString("Could not convert string value '%1' to a number.").arg(powerStr));
        }

        CFRelease(prop);
        prop = nullptr;
    }
    else
    {
        WRN(QString("Property with name '%1' not found.").arg(batteryDataKey));
    }

    DBG(QString("Charge rate is: %1").arg(adapterPower));

    return adapterPower;
}

int PowerInfo::dischargeRate()
{
    DBG_CALLED;

    return 0;
}

int PowerInfo::currentCapacity()
{
    DBG_CALLED;

    if(m_service == 0)
    {
        WRN("No information service created.");
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

        if(!ok)
        {
            WRN(QString("The value '%1' could not be converted into a number.").arg(str));
        }

        CFRelease(prop);
        prop = nullptr;
    }
    else
    {
        WRN(QString("Property with name '%1' not found.").arg(currentCapacityKey));
    }

    DBG(QString("The current capacity is: %1").arg(theCurrentCapacity));
    return theCurrentCapacity;
}
