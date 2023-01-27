#include "logmanager.h"
#include "logsystem.h"

static Log::ILogManager *g_logManager = nullptr;

Log::ILogManager *Log::LogSystem::logManager()
{
    if(g_logManager == nullptr)
    {
        g_logManager = new LogManager;
    }

    return g_logManager;
}

void Log::LogSystem::Destroy()
{
    delete g_logManager;
    g_logManager = nullptr;
}
