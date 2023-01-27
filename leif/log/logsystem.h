#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include "ilogmanager.h"

namespace Log {

class LogSystem
{
public:
    static ILogManager *logManager();
    static void Destroy();
};

}

#endif // !LOGSYSTEM_H
