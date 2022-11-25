/**
 * \brief Implements the CarbonModel class.
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 *
 * \date 17.09.2022
 */
#include "carbonmodel.h"
#include "carbonprocessor.h"

/**
 * @brief Creates a CarbonModel instance.
 *
 * @param parent Optional QObject parent.s
 */
CarbonModel::CarbonModel(QObject *parent /* = nullptr */):
    QObject{parent}
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        connect(carbon, &CarbonProcessor::lifetimeCarbonChanged, this, &CarbonModel::lifetimeCarbonChanged);
        connect(carbon, &CarbonProcessor::sessionCarbonChanged, this, &CarbonModel::sessionCarbonChanged);
        connect(carbon, &CarbonProcessor::carbonUsageLevelChanged, this, &CarbonModel::carbonUsageLevelChanged);
    }
}

/**
 * @brief Returns the session carbon counter value.
 *
 * @return The session carbon count in grams as an \c integer.
 */
float CarbonModel::sessionCarbon() const
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        return carbon->sessionCarbon();
    }

    return 0.0;
}

/**
 * @brief Returns the life time carbon counter value.
 *
 * @return The lifetime carbon count in grams as an \c integer.
 */
float CarbonModel::lifetimeCarbon() const
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        return carbon->lifetimeCarbon();
    }

    return 0.0;
}

/**
 * @brief Returns the carbon usage level.
 *
 * The carbon usage level, as represented by the
 * CarbonProcessor::CarbonUsageLevel, informs about the current carbon cost of
 * a kilowatt hour.
 *
 * @return The carbon cost intensity as a CarbonUsageLevel enum value.
 */
CarbonProcessor::CarbonUsageLevel CarbonModel::carbonUsageLevel() const
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        return carbon->carbonUsageLevel();
    }

    return CarbonProcessor::VeryHigh;
}

/**
 * @brief Clears the stored life time session counter.
 *
 * This method clears the life time session carbon counter. All other instances
 * of the CarbonModel class will notify their respective listeners.
 */
void CarbonModel::clearStats()
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        carbon->clearStats();
    }
}

