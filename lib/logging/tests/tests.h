#pragma once

#define LOGGING_NO_COLORS
#include <logging/logging.h>

#define INITIALIZE()                                                           \
    logging::detail::log_specs.logLevel = logging::level::debug;               \
    logging::detail::log_specs.toFile   = false;                               \
    logging::detail::log_specs.toStdout = true;                                \
    logging::detail::log_specs.stream   = &logging::default_stream;            \
    logging::detail::log_specs.path     = "logs/program.log";                  \
    std::ostringstream str;                                                    \
    std::string time = std::format(                                            \
        "[{:%F %X}]",                                                          \
        std::chrono::zoned_time(std::chrono::current_zone(),                   \
                                std::chrono::system_clock::now()));            \
    logging::set_stdout(str)
