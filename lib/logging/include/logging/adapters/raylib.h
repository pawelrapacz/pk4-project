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

    inline void raylib_callback(int logLevel, const char* text, va_list args) {
        static char buffer[512];
        std::vsnprintf(buffer, 512, text, args);

        switch (logLevel) {
        case LOG_TRACE:
            [[fallthrough]];
        case LOG_DEBUG:
            debug(buffer);
            break;
        case LOG_INFO:
            info(buffer);
            break;
        case LOG_WARNING:
            warn(buffer);
            break;
        case LOG_ERROR:
            error(buffer);
            break;
        case LOG_FATAL:
            fatal(buffer);
            break;
        default:
            break;
        }
    }

#else

    inline void raylib_callback(int, const char*, va_list) { }

#endif

} // namespace logging
