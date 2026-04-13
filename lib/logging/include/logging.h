#pragma once

#include <string_view>

#include "styles.h"

namespace logging {

    enum LogLevel {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        CRITICAL,
    };

    void Log(LogLevel, std::string_view);

}


#ifndef LOGGING_DISABLE_GLOBAL_NAMESPACE
    using namespace logging;
#endif


#ifndef DISABLE_LOGGING
    #define LOG(level, text) logging::Log(level, text)
#else
    #define LOG(text)
#endif

#define DEBUG(text) LOG(logging::LogLevel::DEBUG, text)
#define INFO(text) LOG(logging::LogLevel::INFO, text)
#define WARNING(text) LOG(logging::LogLevel::WARNING, text)
#define ERROR(text) LOG(logging::LogLevel::ERROR, text)
#define CRITICAL(text) LOG(logging::LogLevel::CRITICAL, text)
