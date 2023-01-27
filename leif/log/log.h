#ifndef LOG_H
#define LOG_H

#include "log_internal.h"

#define INF(msg) LOG(Log::MessageType::Information, msg)
#define WRN(msg) LOG(Log::MessageType::Warning, msg)
#define ERR(msg) LOG(Log::MessageType::Error, msg)
#define DBG(msg) LOG(Log::MessageType::Debug, msg)

#define DBG_CALLED DBG("Called")

#endif // LOG_H
