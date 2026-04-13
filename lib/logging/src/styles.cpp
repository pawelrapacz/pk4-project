#include <iostream>

#include "styles.h"

using namespace logging;

void logging::ResetStyle(std::ostream& outstr) {
    outstr << ANSI_CSI "0m";
}

void logging::ApplyStyle(const CLIStyle& style, std::ostream& outstr) {
    outstr << to_ansi(style.fg);
    outstr << to_ansi(style.bg);

    for (auto& i : TEXT_STYLE_MAP) {
        if (std::to_underlying(i.key) & std::to_underlying(style.text))
            outstr << i.seq;
    }
}

std::string logging::StyleSequence(const CLIStyle& style) {
    std::string seq;

    seq += to_ansi(style.fg);
    seq += to_ansi(style.bg);

    for (auto& i : TEXT_STYLE_MAP) {
        if (std::to_underlying(i.key) & std::to_underlying(style.text))
            seq += i.seq;
    }

    return seq;
}