#ifndef LOGFILTERBASE_H
#define LOGFILTERBASE_H

#include "ilogger.h"

namespace Log {

class LogFilterBase : public ILogger
{
public:
    explicit LogFilterBase(ILogger *logger);
    virtual ~LogFilterBase();

    virtual void logMessage(const QString &file,
                            const QString &methodName,
                            const int &codeLine,
                            const MessageType &type,
                            const QString &message) override;

protected:
    virtual bool canLogMessage(const QString &file,
                               const QString &methodName,
                               const int &codeLine,
                               const MessageType &type,
                               const QString &message) const = 0;

private:
    ILogger *m_logger;
};

}
#endif // LOGFILTERBASE_H
