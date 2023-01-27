#include <QIODevice>
#include <QTextStream>

#include "iodevicelogger.h"

class Log::IODeviceLoggerPrivate
{
private:
    IODeviceLoggerPrivate();
    ~IODeviceLoggerPrivate();

    bool initialized;
    QIODevice *device;
    QTextStream out;

    friend class IODeviceLogger;
};

Log::IODeviceLoggerPrivate::IODeviceLoggerPrivate():
    initialized {false},
    device{nullptr}
{}

Log::IODeviceLoggerPrivate::~IODeviceLoggerPrivate()
{
    out.flush();
    out.setDevice(nullptr);

    if(device != nullptr)
    {
        device->close();

        delete device;
        device = nullptr;
    }
}

Log::IODeviceLogger::IODeviceLogger():
    d {new IODeviceLoggerPrivate}
{}

Log::IODeviceLogger::~IODeviceLogger()
{
    delete d;
    d = nullptr;
}

bool Log::IODeviceLogger::isInitialized() const
{
    Q_ASSERT(d != nullptr);

    return d->initialized;
}

void Log::IODeviceLogger::logDigestedMessage(const QString &digestedMessage)
{
    if(!isInitialized())
    {
        initialize();
    }

    writeToTextStream(digestedMessage);
}

void Log::IODeviceLogger::initialize()
{
    Q_ASSERT(d != nullptr);
    Q_ASSERT(!isInitialized());

    d->initialized = true;
    d->device = openDevice();
    d->out.setDevice(d->device);
}

void Log::IODeviceLogger::writeToTextStream(const QString &message)
{
    Q_ASSERT(d != nullptr);

    if(d->device != nullptr && d->device->isOpen())
    {
        d->out << message << Qt::endl;
    }
}
