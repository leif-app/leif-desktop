#ifndef LOGFILTERBYFILE_H
#define LOGFILTERBYFILE_H

#include <QString>

#include "logfilterbase.h"

namespace Log {

class LogFilterByFile : public LogFilterBase
{
public:
    explicit LogFilterByFile(const QString &fileName, ILogger *logger);
    virtual ~LogFilterByFile() = default;

protected:
    virtual bool canLogMessage(const QString &file,
                               const QString &methodName,
                               const int &codeLine,
                               const MessageType &type,
                               const QString &message) const override;

private:
    QString m_fileName;
};

}

#endif // LOGFILTERBYFILE_H
