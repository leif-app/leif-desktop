#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>

#include "messagetype.h"

namespace Log
{

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void logMessage(const QString &file,
                            const QString &methodName,
                            const int &codeLine,
                            const MessageType &type,
                            const QString &message) = 0;
};

}

#endif // ILOGGER_H
