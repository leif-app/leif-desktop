/**
 * @brief Defines the static PowerFactory class.
 *
 * The PowerFactory class creates a IPower interface implementation on each
 * system which in turn will tell us how much power a computer draws.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
#ifndef POWERFACTORY_H
#define POWERFACTORY_H

#include <interfaces/IPower.h>

class SettingsService;

class PowerFactory
{
public:
    static std::unique_ptr<IPower> getPowerInterface(SettingsService *settings);
};

#endif // POWERFACTORY_H
