/**
 * \file carbonusagelevel.h
 *
 * \brief Defines the CarbonUsageLevel enum class.
 *
 * The usage level is used to indicate how much CO2 is being used. We use this
 * enum to display the correct icon in the tray bar for example.
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 */
#ifndef CARBONUSAGELEVEL_H
#define CARBONUSAGELEVEL_H

enum class CarbonUsageLevel
{
    VeryLow,
    Low,
    Medium,
    High,
    VeryHigh
};

#endif // !CARBONUSAGELEVEL_H
