#pragma once

#include <format>
#include <iostream>
#include <string_view>

#include "styles.h"

namespace logging {

    enum LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
    };

    void Log(LogLevel lv, std::string_view msg, std::ostream& outstr = std::clog);

}


#ifndef LOGGING_DISABLE_GLOBAL_NAMESPACE
    using namespace logging;
#endif

#ifndef DISABLE_LOGGING
    #define LOG(level, fmt, ...) logging::Log(level, std::format(fmt __VA_OPT__(,) __VA_ARGS__))
#else
    #define LOG(...)
#endif

#if defined(_DEBUG) || !defined(NDEBUG) || defined(LOGGING_FORCE_USE_DEBUG)
    #define DEBUG(fmt, ...) LOG(logging::LogLevel::DEBUG, "{}:{} {}", __FILE__, __LINE__, std::format(fmt __VA_OPT__(,) __VA_ARGS__))
#else
    #define DEBUG(...)
#endif


#define INFO(fmt, ...) LOG(logging::LogLevel::INFO, fmt __VA_OPT__(,) __VA_ARGS__)
#define WARN(fmt, ...) LOG(logging::LogLevel::WARN, fmt __VA_OPT__(,) __VA_ARGS__)
#define ERROR(fmt, ...) LOG(logging::LogLevel::ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
#define FATAL(fmt, ...) LOG(logging::LogLevel::FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
