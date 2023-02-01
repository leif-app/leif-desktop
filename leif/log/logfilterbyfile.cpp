#include "logfilterbyfile.h"

Log::LogFilterByFile::LogFilterByFile(const QString &fileName, ILogger *logger):
    LogFilterBase {logger},
    m_fileName {fileName}
{}

bool Log::LogFilterByFile::canLogMessage(const QString &file,
                                         const QString &methodName,
                                         const int &codeLine,
                                         const MessageType &type,
                                         const QString &message) const
{
    Q_UNUSED(methodName);
    Q_UNUSED(codeLine);
    Q_UNUSED(type);
    Q_UNUSED(message);

    return file.contains(m_fileName);
}
