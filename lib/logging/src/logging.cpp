#include <iostream>
#include <array>
#include <chrono>
#include <format>
#include <fstream>

#include "logging.h"

constexpr auto STYLES = std::to_array<CLIStyle>({
    FG_BLUE | ITALIC, // DEBUG
    {}, // INFO
    FG_YELLOW, // WARN
    FG_RED | BOLD, // ERROR
    BG_RED | BOLD, // FATAL
});

constexpr auto LABELS = std::to_array<std::string_view>({
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
});


void logging::Log(LogLevel lv, std::string_view msg, std::ostream& outstr) {
    ResetStyle();

    ApplyStyle(STYLES[static_cast<std::size_t>(lv)], outstr);

    auto time = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());
    auto str =std::format("[{:%F %X}] {} {}", time, LABELS[static_cast<std::size_t>(lv)], msg);

    outstr << str;
    ResetStyle(outstr);
    outstr << std::endl;

    static std::ofstream file("log.log");

    file << str << std::endl;

}
