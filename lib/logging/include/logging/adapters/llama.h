#pragma once

#ifndef LOGGING_LLAMA_ADAPTER
    #error "To use this header file enable the LOGGING_LLAMA_ADAPTER option."
#endif

#include <llama.h>
#include <logging/logging.h>

namespace logging {

#ifndef LOGGING_DISABLE

    namespace detail {
        inline logging::level llama_log_level = level {};
    }

    inline void set_llama_level(level lvl) noexcept {
        detail::llama_log_level = lvl;
    }

    inline void llama_callback(ggml_log_level logLevel, const char* text,
                               void*) {
        static level realLevel = level::info;
        switch (logLevel) {
        case GGML_LOG_LEVEL_NONE:
            [[fallthrough]];
        case GGML_LOG_LEVEL_INFO:
            realLevel = level::info;
            break;
        case GGML_LOG_LEVEL_DEBUG:
            realLevel = level::debug;
            break;
        case GGML_LOG_LEVEL_WARN:
            realLevel = level::warn;
            break;
        case GGML_LOG_LEVEL_ERROR:
            realLevel = level::error;
            break;
        default: // GGML_LOG_LEVEL_CONT - continue previous log
            break;
        }

        if (realLevel < detail::llama_log_level)
            return;
        else
            log(realLevel, text);
    }

#else

    inline void set_llama_level(level) noexcept { }

    inline void llama_callback(ggml_log_level, const char*, void*) { }

#endif

} // namespace logging
