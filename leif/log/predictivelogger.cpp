#include <QDateTime>

#include "predictivelogger.h"

class Log::PredictiveLoggerPrivate
{
    PredictiveLoggerPrivate();
    ~PredictiveLoggerPrivate() = default;

    QStringList buffer;
    bool errorMode;
    QDateTime errorModeUntill;

    friend class PredictiveLogger;
};

Log::PredictiveLoggerPrivate::PredictiveLoggerPrivate():
    errorMode {false}
{}

Log::PredictiveLogger::PredictiveLogger():
    d {new PredictiveLoggerPrivate}
{}

Log::PredictiveLogger::~PredictiveLogger()
{
    delete d;
    d = nullptr;
}

void Log::PredictiveLogger::logMessage(const QString &file,
                                       const QString &methodName,
                                       const int &codeLine,
                                       const MessageType &type,
                                       const QString &message)
{
    Q_ASSERT(d != nullptr);

    checkErrorMode();

    if(inErrorMode())
    {
        FileLogger::logMessage(file, methodName, codeLine, type, message);
        return;
    }

    storeMessage(makeMessage(file, methodName, codeLine, type, message));
}

void Log::PredictiveLogger::onError()
{
    Q_ASSERT(d != nullptr);

    d->errorMode = true;
    d->errorModeUntill = QDateTime::currentDateTime().addMSecs(1000 * 60 * 5);

    dumpStoredMessages();
}

bool Log::PredictiveLogger::inErrorMode()
{
    Q_ASSERT(d != nullptr);

    return d->errorMode;
}

void Log::PredictiveLogger::checkErrorMode()
{
    Q_ASSERT(d != nullptr);

    if(!inErrorMode())
    {
        return;
    }

    if(d->errorModeUntill <= QDateTime::currentDateTime())
    {
        d->errorMode = false;
    }
}

void Log::PredictiveLogger::dumpStoredMessages()
{
    Q_ASSERT(d != nullptr);

    for(int i = 0; i < d->buffer.count(); ++i)
    {
        QString message = d->buffer.at(i);
        logDigestedMessage(message);
    }

    d->buffer.clear();
}

void Log::PredictiveLogger::storeMessage(const QString &digestedMessage)
{
    Q_ASSERT(d != nullptr);

    d->buffer << digestedMessage;

    if(d->buffer.count() > 100)
    {
        d->buffer.removeFirst();
    }
}
