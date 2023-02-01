#include "logfilterbase.h"

Log::LogFilterBase::LogFilterBase(ILogger *logger):
    m_logger {logger}
{}

Log::LogFilterBase::~LogFilterBase()
{
    delete m_logger;
    m_logger = nullptr;
}

void Log::LogFilterBase::logMessage(const QString &file,
                                    const QString &methodName,
                                    const int &codeLine,
                                    const MessageType &type,
                                    const QString &message)
{
    if(m_logger == nullptr || !canLogMessage(file, methodName, codeLine, type, message))
    {
        return;
    }

    m_logger->logMessage(file, methodName, codeLine, type, message);
}
