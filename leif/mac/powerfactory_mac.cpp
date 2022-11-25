/**
 * @brief Implements the PowerFactory methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
#include "powerfactory.h"
#include "mac/powerinfo.h"

IPower * PowerFactory::getPowerInterface()
{
    return new PowerInfo;
}
