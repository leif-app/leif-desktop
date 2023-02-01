#ifndef PREDICTIVELOGGER_H
#define PREDICTIVELOGGER_H

#include "filelogger.h"

namespace Log {

class PredictiveLoggerPrivate;

class PredictiveLogger : public FileLogger
{
public:
    PredictiveLogger();
    virtual ~PredictiveLogger();

    virtual void logMessage(const QString &file, const QString &methodName,
                            const int &codeLine, const MessageType &type,
                            const QString &message) override;

private:
    void onError();
    bool inErrorMode();
    void checkErrorMode();
    void dumpStoredMessages();
    void storeMessage(const QString &digestedMessage);

private:
    Q_DISABLE_COPY_MOVE(PredictiveLogger);
    PredictiveLoggerPrivate *d;
};

}

#endif // PREDICTIVELOGGER_H
