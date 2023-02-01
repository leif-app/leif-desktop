#include <QDebug>
#include "consolelogger.h"

void Log::ConsoleLogger::logDigestedMessage(const QString &digestedMessage)
{
    qDebug() << digestedMessage;
}
