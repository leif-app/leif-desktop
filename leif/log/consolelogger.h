#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "loggerbase.h"

namespace Log {
class ConsoleLogger : public LoggerBase
{
public:
    virtual ~ConsoleLogger() = default;

protected:
    virtual void logDigestedMessage(const QString &digestedMessage) override;
};
}

#endif // CONSOLELOGGER_H
