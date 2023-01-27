#ifndef IODEVICELOGGER_H
#define IODEVICELOGGER_H

#include <QTextStream>

#include "loggerbase.h"

class QIODevice;

namespace Log
{
class IODeviceLoggerPrivate;

class IODeviceLogger : public LoggerBase
{
public:
    IODeviceLogger();
    virtual ~IODeviceLogger();

    bool isInitialized() const;

protected:
    virtual void logDigestedMessage(const QString &digestedMessage) override;
    virtual QIODevice *openDevice() = 0;

private:
    void initialize();
    void writeToTextStream(const QString &message);

private:
    Q_DISABLE_COPY_MOVE(IODeviceLogger)
    IODeviceLoggerPrivate *d;
};

}

#endif // IODEVICELOGGER_H
