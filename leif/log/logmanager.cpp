#include <QList>

#include "logmanager.h"

class Log::LogManagerPrivate
{
private:
    LogManagerPrivate() = default;
    ~LogManagerPrivate();

    QList<ILogger*> logger;
    friend class LogManager;
};

Log::LogManagerPrivate::~LogManagerPrivate()
{
    qDeleteAll(logger);
    logger.clear();
}

Log::LogManager::LogManager():
    d{new LogManagerPrivate}
{}

Log::LogManager::~LogManager()
{
    delete d;
    d = nullptr;
}

void Log::LogManager::logMessage(const QString &file,
                                 const int &codeLine,
                                 const MessageType &type,
                                 const QString &message)
{
    Q_ASSERT(d != nullptr);

    for(int i = 0; i < d->logger.count(); ++i)
    {
        d->logger.at(i)->logMessage(file, codeLine, type, message);
    }
}

void Log::LogManager::registerLogger(ILogger *logger)
{
    Q_ASSERT(d != nullptr);

    if(!d->logger.contains(logger))
    {
        d->logger << logger;
    }
}
