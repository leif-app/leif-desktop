/**
 * @brief Implements the PowerFactory methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
/**
 * @brief Implements the PowerFactory methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
#include "settingsservice.h"
#include "powerfactory.h"
#include "mac/powerinfo.h"

std::unique_ptr<IPower> PowerFactory::getPowerInterface(SettingsService *settings)
{
    if(settings == nullptr)
        return nullptr;

    auto storeFunc = [=](int avgDischargeRate)
    {
        settings->saveAverageDischargeRate(avgDischargeRate);
    };

    return std::make_unique<PowerInfo>(settings->averageDischargeRate(), storeFunc);
}
