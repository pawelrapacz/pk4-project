#include <iostream>
#include <chrono>

#include "logging.h"



void logging::Log(LogLevel lv, std::string_view text) {
    ResetStyle();

    switch (lv) {
        case LogLevel::WARNING:
            ApplyStyle(FG_YELLOW);
            break;
        case LogLevel::ERROR:
            ApplyStyle(FG_RED);
            break;
        case LogLevel::CRITICAL:
            ApplyStyle(BOLD | BG_RED);
            break;
        case LogLevel::DEBUG:
            ApplyStyle(FG_YELLOW);
            break;
        default:
    }

    std::cout << text;
    ResetStyle();
    std::cout << std::endl;
}
