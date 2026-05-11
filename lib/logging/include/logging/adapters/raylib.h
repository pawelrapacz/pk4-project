#pragma once

#ifndef LOGGING_RAYLIB_ADAPTER
    #error "To use this header file enable the LOGGING_RAYLIB_ADAPTER option."
#endif

#include <logging/logging.h>
#include <raylib.h>

#include <cstdarg>
#include <cstdio>

namespace logging {

#ifndef LOGGING_DISABLE

    namespace detail {
        inline logging::level raylib_log_level = level {};
    }

    inline void set_raylib_level(level lvl) noexcept {
        detail::raylib_log_level = lvl;
    }

    inline void raylib_callback(int logLevel, const char* text, va_list args) {
        static char buffer[512];
        std::vsnprintf(buffer, 512, text, args);

        level realLevel;
        switch (logLevel) {
        case LOG_TRACE:
            [[fallthrough]];
        case LOG_DEBUG:
            realLevel = level::debug;
            break;
        case LOG_NONE:
            [[fallthrough]];
        case LOG_INFO:
            realLevel = level::info;
            break;
        case LOG_WARNING:
            realLevel = level::warn;
            break;
        case LOG_ERROR:
            realLevel = level::error;
            break;
        case LOG_FATAL:
            realLevel = level::fatal;
            break;
        default:
            return;
        }

        if (realLevel < detail::raylib_log_level)
            return;
        else
            log(realLevel, buffer);
    }

#else

    inline void set_raylib_level(level) noexcept { }

    inline void raylib_callback(int, const char*, va_list) { }

#endif

} // namespace logging
