#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <utility>
#include <iostream>


#define ANSI_ESC "\x1b"

#define ANSI_CSI ANSI_ESC "["

namespace logging {

    enum ANSITextStyle : std::uint32_t {
        BOLD = 1 << 0,
        FAINT = 1 << 1,
        ITALIC = 1 << 2,
        UNDERLINE = 1 << 3,
        LINE_THROUGH = 1 << 4,
        DOUBLE_UNDERLINE = 1 << 5,
        OVERLINE = 1 << 6,
    };

    struct ANSITextEntry {
        ANSITextStyle key;
        std::string_view seq;
    };

    constexpr auto TEXT_STYLE_MAP = std::to_array<ANSITextEntry>({
        {ANSITextStyle::BOLD, ANSI_CSI "1m"},
        {ANSITextStyle::FAINT, ANSI_CSI "2m"},
        {ANSITextStyle::ITALIC, ANSI_CSI "3m"},
        {ANSITextStyle::UNDERLINE, ANSI_CSI "4m"},
        {ANSITextStyle::LINE_THROUGH, ANSI_CSI "9m"},
        {ANSITextStyle::DOUBLE_UNDERLINE, ANSI_CSI "21m"},
        {ANSITextStyle::OVERLINE, ANSI_CSI "53m"},
    });

    enum ANSIColor : std::uint32_t {
        NONE_ANSI_CLR,

        // Foreground (font) color
        FG_RESET,
        FG_BLACK,
        FG_RED,
        FG_GREEN,
        FG_YELLOW,
        FG_BLUE,
        FG_MAGENTA,
        FG_CYAN,
        FG_WHITE,
        FG_GRAY,
        FG_BRIGHT_RED,
        FG_BRIGHT_GREEN,
        FG_BRIGHT_YELLOW,
        FG_BRIGHT_BLUE,
        FG_BRIGHT_MAGENTA,
        FG_BRIGHT_CYAN,
        FG_BRIGHT_WHITE,

        // Background color
        BG_RESET,
        BG_BLACK,
        BG_RED,
        BG_GREEN,
        BG_YELLOW,
        BG_BLUE,
        BG_MAGENTA,
        BG_CYAN,
        BG_WHITE,
        BG_GRAY,
        BG_BRIGHT_RED,
        BG_BRIGHT_GREEN,
        BG_BRIGHT_YELLOW,
        BG_BRIGHT_BLUE,
        BG_BRIGHT_MAGENTA,
        BG_BRIGHT_CYAN,
        BG_BRIGHT_WHITE,
    };

    constexpr auto COLORS_TABLE = std::to_array<std::string_view>({
        "",

        // Foreground (font) color
        ANSI_CSI "39m",  // FG_RESET
        ANSI_CSI "30m",  // FG_BLACK
        ANSI_CSI "31m",  // FG_RED
        ANSI_CSI "32m",  // FG_GREEN
        ANSI_CSI "33m",  // FG_YELLOW
        ANSI_CSI "34m",  // FG_BLUE
        ANSI_CSI "35m",  // FG_MAGENTA
        ANSI_CSI "36m",  // FG_CYAN
        ANSI_CSI "37m",  // FG_WHITE
        ANSI_CSI "90m",  // FG_GRAY
        ANSI_CSI "91m",  // FG_BRIGHT_RED
        ANSI_CSI "92m",  // FG_BRIGHT_GREEN
        ANSI_CSI "93m",  // FG_BRIGHT_YELLOW
        ANSI_CSI "94m",  // FG_BRIGHT_BLUE
        ANSI_CSI "95m",  // FG_BRIGHT_MAGENTA
        ANSI_CSI "96m",  // FG_BRIGHT_CYAN
        ANSI_CSI "97m",  // FG_BRIGHT_WHITE

        // Background color
        ANSI_CSI "49m",   // BG_RESET
        ANSI_CSI "40m",   // BG_BLACK
        ANSI_CSI "41m",   // BG_RED
        ANSI_CSI "42m",   // BG_GREEN
        ANSI_CSI "43m",   // BG_YELLOW
        ANSI_CSI "44m",   // BG_BLUE
        ANSI_CSI "45m",   // BG_MAGENTA
        ANSI_CSI "46m",   // BG_CYAN
        ANSI_CSI "47m",   // BG_WHITE
        ANSI_CSI "100m",  // BG_GRAY
        ANSI_CSI "101m",  // BG_BRIGHT_RED
        ANSI_CSI "102m",  // BG_BRIGHT_GREEN
        ANSI_CSI "103m",  // BG_BRIGHT_YELLOW
        ANSI_CSI "104m",  // BG_BRIGHT_BLUE
        ANSI_CSI "105m",  // BG_BRIGHT_MAGENTA
        ANSI_CSI "106m",  // BG_BRIGHT_CYAN
        ANSI_CSI "107m",  // BG_BRIGHT_WHITE
    });


    constexpr std::string_view to_ansi(ANSITextStyle code) noexcept {
        switch (code) {
            case ANSITextStyle::BOLD: return ANSI_CSI "1m";
            case ANSITextStyle::FAINT: return ANSI_CSI "2m";
            case ANSITextStyle::ITALIC: return ANSI_CSI "3m";
            case ANSITextStyle::UNDERLINE: return ANSI_CSI "4m";
            case ANSITextStyle::LINE_THROUGH: return ANSI_CSI "9m";
            case ANSITextStyle::DOUBLE_UNDERLINE: return ANSI_CSI "21m";
            case ANSITextStyle::OVERLINE: return ANSI_CSI "53m";
            default: return {};
        }
    }

    constexpr std::string_view to_ansi(ANSIColor clrCode) noexcept {
        return COLORS_TABLE[static_cast<std::size_t>(clrCode)];
    }


    struct CLIStyle {
        ANSITextStyle text { };
        ANSIColor fg { };
        ANSIColor bg { };

        constexpr CLIStyle() = default;

        constexpr CLIStyle(ANSITextStyle code) noexcept
            : text(code) { }

        constexpr CLIStyle(ANSIColor code) noexcept {
            impl_set_appropriate_color(code);
        }


        constexpr inline void impl_set_appropriate_color(ANSIColor clrCode) noexcept {
            if (clrCode < ANSIColor::BG_RESET)
                fg = clrCode;
            else
                bg = clrCode;
        }
    };

    constexpr CLIStyle operator|(ANSITextStyle code1, ANSITextStyle code2) noexcept {
        return {static_cast<ANSITextStyle>(
            std::to_underlying(code1) | std::to_underlying(code2)
        )};
    }

    constexpr CLIStyle operator|(ANSIColor clrCode, ANSITextStyle txtCode) noexcept {
        CLIStyle style;
        style.text = txtCode;
        style.impl_set_appropriate_color(clrCode);
        return style;
    }

    constexpr CLIStyle operator|(ANSITextStyle txtCode, ANSIColor clrCode) noexcept {
        return operator|(clrCode, txtCode);
    }

    constexpr CLIStyle operator|(const CLIStyle& style, ANSITextStyle txtCode) noexcept {
        CLIStyle newStyle = style;
        newStyle.text = static_cast<ANSITextStyle>(
            std::to_underlying(style.text) | std::to_underlying(txtCode)
        );
        return newStyle;
    }

    constexpr CLIStyle operator|(const CLIStyle& style, ANSIColor clrCode) noexcept {
        CLIStyle newStyle = style;
        newStyle.impl_set_appropriate_color(clrCode);
        return newStyle;
    }

    void ResetStyle(std::ostream& outstr = std::cout);

    void ApplyStyle(const CLIStyle& style, std::ostream& outstr = std::cout);

    std::string StyleSequence(const CLIStyle& style);

}