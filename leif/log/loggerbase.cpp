#include <QDateTime>

#include "loggerbase.h"

void Log::LoggerBase::logMessage(const QString &file,
                                 const int &codeLine,
                                 const MessageType &type,
                                 const QString &message)
{
    QString toLog = makeMessage(file, codeLine, type, message);

    logDigestedMessage(toLog);
}

QString Log::LoggerBase::makeMessage(const QString &file,
                                     const int &codeLine,
                                     const MessageType &type,
                                     const QString &message)
{
    QString format = QStringLiteral("%1 [%2] (%3) - %4");

    QString digestedMessage = format.arg(timeStamp(),
                                         typeToString(type),
                                         location(file, codeLine),
                                         message);

    return digestedMessage;
}

QString Log::LoggerBase::location(const QString &file, const int &codeLine)
{
    QString format = QStringLiteral("%1:%2");

    QString locationString = format.arg(file.right(60), QString::number(codeLine));

    return locationString;
}

QString Log::LoggerBase::timeStamp()
{
    QString format = QStringLiteral("ddMMyyyy hh:mm:ss.zzz");

    return QDateTime::currentDateTime().toString(format);
}

QString Log::LoggerBase::typeToString(const MessageType &type)
{
    QString str;

    switch(type)
    {
    case MessageType::Information:
        str = QStringLiteral("INF");
        break;

    case MessageType::Warning:
        str = QStringLiteral("WRN");
        break;

    case MessageType::Error:
        str = QStringLiteral("ERR");
        break;

    case MessageType::Debug:
        str = QStringLiteral("DBG");
        break;
    }

    return str;
}
