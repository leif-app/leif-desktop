/**
 * \brief Implements the CarbonModel class.
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 *
 * \date 17.09.2022
 */
#include "carboncontroller.h"
#include "carbonservice.h"

/**
 * @brief Creates a CarbonModel instance.
 *
 * @param parent Optional QObject parent.s
 */
CarbonController::CarbonController(CarbonService *carbonService, QObject *parent /* = nullptr */):
    QObject{parent}
{
    d.carbonService = carbonService;

    if(d.carbonService != nullptr)
    {
        connect(d.carbonService, &CarbonService::lifetimeCarbonChanged, this, &CarbonController::lifetimeCarbonChanged);
        connect(d.carbonService, &CarbonService::sessionCarbonChanged, this, &CarbonController::sessionCarbonChanged);
        connect(d.carbonService, &CarbonService::carbonUsageLevelChanged, this, &CarbonController::carbonUsageLevelChanged);
    }
}

/**
 * @brief Returns the session carbon counter value.
 *
 * @return The session carbon count in grams as an \c integer.
 */
float CarbonController::sessionCarbon() const
{
    if(d.carbonService != nullptr)
        return d.carbonService->sessionCarbon();

    return 0.0;
}

/**
 * @brief Returns the life time carbon counter value.
 *
 * @return The lifetime carbon count in grams as an \c integer.
 */
float CarbonController::lifetimeCarbon() const
{
    if(d.carbonService != nullptr)
        return d.carbonService->lifetimeCarbon();

    return 0.0;
}

/**
 * @brief Returns the carbon usage level.
 *
 * The carbon usage level, as represented by the
 * CarbonService::CarbonUsageLevel, informs about the current carbon cost of
 * a kilowatt hour.
 *
 * @return The carbon cost intensity as a CarbonUsageLevel enum value.
 */
CarbonUsageLevel CarbonController::carbonUsageLevel() const
{
    if(d.carbonService != nullptr)
        return d.carbonService->carbonUsageLevel();
    
    return CarbonUsageLevel::VeryHigh;
}

/**
 * @brief Returns the current charge forecast.
 *
 * The charge forecast, as represented by the CarbonService::ChargeForecast
 * value informs the user about the best time to charge the device.
 *
 * @return The charge forecast value as an CarbonService::ChargeForecast value.
 */
ChargeForecast CarbonController::chargeForecast() const
{
    if(d.carbonService != nullptr)
    {
        return d.carbonService->chargeForecast();
    }
    
    return ChargeForecast::ChargeWhenNeeded;
}

/**
 * @brief Clears the stored life time session counter.
 *
 * This method clears the life time session carbon counter. All other instances
 * of the CarbonModel class will notify their respective listeners.
 */
void CarbonController::clearStats()
{
    if(d.carbonService != nullptr)
    {
        d.carbonService->clearStats();
    }
}

