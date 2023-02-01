#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "ilogmanager.h"

namespace Log
{
class LogManagerPrivate;

class LogManager : public ILogManager
{
public:
    LogManager();
    virtual ~LogManager();

    virtual void logMessage(const QString &file,
                            const QString &methodName,
                            const int &codeLine,
                            const MessageType &type,
                            const QString &message) override;

    virtual void registerLogger(ILogger *logger) override;

private:
    Q_DISABLE_COPY_MOVE(LogManager)
    LogManagerPrivate *d;
};
}

#endif // !LOGMANAGER_H
