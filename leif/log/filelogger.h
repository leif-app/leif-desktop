#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "iodevicelogger.h"

namespace Log
{

class FileLogger : public IODeviceLogger
{
public:
    FileLogger() = default;
    virtual ~FileLogger() = default;

protected:
    virtual QIODevice *openDevice() override;
};

}

#endif // FILELOGGER_H
