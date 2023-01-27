#ifndef ILOGMANAGER_H
#define ILOGMANAGER_H

#include "ilogger.h"

namespace Log
{
class ILogManager : public ILogger
{
public:
    virtual ~ILogManager() = default;

    virtual void registerLogger(ILogger *logger) = 0;
};
}

#endif // ILOGMANAGER_H
