#include <Windows.h>
#include <ntstatus.h>
#include <powerbase.h>
#include <devguid.h>
#include <batclass.h>

#include "powerinfo.h"

#define BATTERY_UNKNOWN_RATE    0x80000000

namespace PowerInfoHelper
{
    static bool hasBattery(PSYSTEM_BATTERY_STATE pBatteryState);
    static bool batteryCharging(PSYSTEM_BATTERY_STATE pBatteryState);
    static bool batteryFullyCharged(PSYSTEM_BATTERY_STATE pBatteryState);
}

bool PowerInfoHelper::hasBattery(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    return pBatteryState->BatteryPresent;
}

bool PowerInfoHelper::batteryCharging(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    return pBatteryState->Charging;
}

bool PowerInfoHelper::batteryFullyCharged(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    if(!pBatteryState->AcOnLine)
    {
        return false;
    }

    if(pBatteryState->Rate != BATTERY_UNKNOWN_RATE || pBatteryState->Rate == 0)
    {
        return true;
    }

    return pBatteryState->Discharging != FALSE;
}


PowerInfo::PowerInfo(QObject *parent /* = nullptr */):
    PowerInfoBase(parent)
{}

bool PowerInfo::hasBattery()
{
    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
    if(result != STATUS_SUCCESS)
    {
        return false;
    }

    return PowerInfoHelper::hasBattery(&batteryState);
}

bool PowerInfo::batteryFullyCharged()
{
    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
    if(result != STATUS_SUCCESS)
    {
        return false;
    }

    return PowerInfoHelper::batteryFullyCharged(&batteryState);
}

bool PowerInfo::batteryCharging()
{
    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
    if(result != STATUS_SUCCESS)
    {
        return false;
    }

    return PowerInfoHelper::batteryCharging(&batteryState);
}

int PowerInfo::chargeRate()
{
    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
    if(result != STATUS_SUCCESS)
    {
        return 0;
    }

    return batteryState.Rate;
}

int PowerInfo::dischargeRate()
{
    return chargeRate();
}

int PowerInfo::currentCapacity()
{
    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));
    if(result != STATUS_SUCCESS)
    {
        return 0;
    }

    return batteryState.RemainingCapacity;
}
