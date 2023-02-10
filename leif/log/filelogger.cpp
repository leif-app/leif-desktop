#include <QFile>

#include "filelogger.h"

QIODevice *Log::FileLogger::openDevice()
{
    QFile *logFile = {new QFile(QStringLiteral("leiflog.txt"))};

    if(logFile->open(QFile::WriteOnly | QFile::Append | QFile::Text))
    {
        return logFile;
    }
    else
    {
        qWarning("Could not open the logfile.");
    }

    delete logFile;
    logFile = nullptr;

    return nullptr;
}
