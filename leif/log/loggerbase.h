#ifndef LOGGERBASE_H
#define LOGGERBASE_H

#include "ilogger.h"

namespace Log
{

class LoggerBase : public ILogger
{
public:
    virtual ~LoggerBase() = default;

    virtual void logMessage(const QString &file,
                            const int &codeLine,
                            const MessageType &type,
                            const QString &message) override;

protected:
    virtual void logDigestedMessage(const QString &digestedMessage) = 0;

private:
    static QString makeMessage(const QString &file,
                               const int &codeLine,
                               const MessageType &type,
                               const QString &message);
    static QString location(const QString &file, const int &codeLine);
    static QString timeStamp();
    static QString typeToString(const MessageType &type);
};

}

#endif // LOGGERBASE_H
