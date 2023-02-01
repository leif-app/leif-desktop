#ifndef LOG_INTERNAL_H
#define LOG_INTERNAL_H

#include "logsystem.h"

#define LOG(type, message) Log::LogSystem::logManager()->logMessage(__FILE__, __FUNCTION__, __LINE__, type, message)

#endif // LOG_INTERNAL_H
