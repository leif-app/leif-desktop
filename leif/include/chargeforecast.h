/**
 * \file chargeforecast.h
 *
 * \brief Defines the ChargeForecast enum class.
 *
 * The ChargeForecast enum class is used to defined various forecast values that
 * are used to display hints to the user.
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 */
#ifndef CHARGEFORECAST_H
#define CHARGEFORECAST_H

enum class ChargeForecast
{
    ChargeNow,
    ChargeIn30,
    ChargeIn60,
    ChargeWhenNeeded
};

#endif // !CHARGEFORECAST_H
