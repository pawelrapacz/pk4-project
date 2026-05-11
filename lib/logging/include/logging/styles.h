#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>


#define ANSI_ESC "\x1b"
#define ANSI_CSI ANSI_ESC "["

namespace logging {
    namespace styles {

        enum text_style : std::uint32_t {
            BOLD             = 1 << 0,
            FAINT            = 1 << 1,
            ITALIC           = 1 << 2,
            UNDERLINE        = 1 << 3,
            LINE_THROUGH     = 1 << 4,
            DOUBLE_UNDERLINE = 1 << 5,
            OVERLINE         = 1 << 6,
        };

        enum color : std::uint32_t {
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

        namespace detail {

            struct text_style_entry {
                text_style key;
                std::string_view seq;
            };

            constexpr auto TEXT_STYLE_MAP = std::to_array<text_style_entry>({
                {            text_style::BOLD,  ANSI_CSI "1m"},
                {           text_style::FAINT,  ANSI_CSI "2m"},
                {          text_style::ITALIC,  ANSI_CSI "3m"},
                {       text_style::UNDERLINE,  ANSI_CSI "4m"},
                {    text_style::LINE_THROUGH,  ANSI_CSI "9m"},
                {text_style::DOUBLE_UNDERLINE, ANSI_CSI "21m"},
                {        text_style::OVERLINE, ANSI_CSI "53m"},
            });

            constexpr auto COLORS_TABLE = std::to_array<std::string_view>({
                "",

                // Foreground (font) color
                ANSI_CSI "39m", // FG_RESET
                ANSI_CSI "30m", // FG_BLACK
                ANSI_CSI "31m", // FG_RED
                ANSI_CSI "32m", // FG_GREEN
                ANSI_CSI "33m", // FG_YELLOW
                ANSI_CSI "34m", // FG_BLUE
                ANSI_CSI "35m", // FG_MAGENTA
                ANSI_CSI "36m", // FG_CYAN
                ANSI_CSI "37m", // FG_WHITE
                ANSI_CSI "90m", // FG_GRAY
                ANSI_CSI "91m", // FG_BRIGHT_RED
                ANSI_CSI "92m", // FG_BRIGHT_GREEN
                ANSI_CSI "93m", // FG_BRIGHT_YELLOW
                ANSI_CSI "94m", // FG_BRIGHT_BLUE
                ANSI_CSI "95m", // FG_BRIGHT_MAGENTA
                ANSI_CSI "96m", // FG_BRIGHT_CYAN
                ANSI_CSI "97m", // FG_BRIGHT_WHITE

                // Background color
                ANSI_CSI "49m",  // BG_RESET
                ANSI_CSI "40m",  // BG_BLACK
                ANSI_CSI "41m",  // BG_RED
                ANSI_CSI "42m",  // BG_GREEN
                ANSI_CSI "43m",  // BG_YELLOW
                ANSI_CSI "44m",  // BG_BLUE
                ANSI_CSI "45m",  // BG_MAGENTA
                ANSI_CSI "46m",  // BG_CYAN
                ANSI_CSI "47m",  // BG_WHITE
                ANSI_CSI "100m", // BG_GRAY
                ANSI_CSI "101m", // BG_BRIGHT_RED
                ANSI_CSI "102m", // BG_BRIGHT_GREEN
                ANSI_CSI "103m", // BG_BRIGHT_YELLOW
                ANSI_CSI "104m", // BG_BRIGHT_BLUE
                ANSI_CSI "105m", // BG_BRIGHT_MAGENTA
                ANSI_CSI "106m", // BG_BRIGHT_CYAN
                ANSI_CSI "107m", // BG_BRIGHT_WHITE
            });

        } // namespace detail


        inline constexpr std::string_view to_ansi(text_style code) noexcept {
            switch (code) {
            case text_style::BOLD:
                return ANSI_CSI "1m";
            case text_style::FAINT:
                return ANSI_CSI "2m";
            case text_style::ITALIC:
                return ANSI_CSI "3m";
            case text_style::UNDERLINE:
                return ANSI_CSI "4m";
            case text_style::LINE_THROUGH:
                return ANSI_CSI "9m";
            case text_style::DOUBLE_UNDERLINE:
                return ANSI_CSI "21m";
            case text_style::OVERLINE:
                return ANSI_CSI "53m";
            default:
                return {};
            }
        }

        inline constexpr std::string_view to_ansi(color clrCode) noexcept {
            return detail::COLORS_TABLE[static_cast<std::size_t>(clrCode)];
        }


        struct style {
            text_style text {};
            color fg {};
            color bg {};

            constexpr style() = default;

            inline constexpr style(text_style code) noexcept
                : text(code) { }

            inline constexpr style(color code) noexcept { set_color_(code); }


            inline constexpr void set_color_(color clrCode) noexcept {
                if (clrCode < color::BG_RESET)
                    fg = clrCode;
                else
                    bg = clrCode;
            }
        };

        inline constexpr style operator|(text_style code1,
                                         text_style code2) noexcept {
            return {static_cast<text_style>(std::to_underlying(code1)
                                            | std::to_underlying(code2))};
        }

        inline constexpr style operator|(color clr1, color clr2) noexcept {
            style style;
            style.set_color_(clr1);
            style.set_color_(clr2);
            return style;
        }

        inline constexpr style operator|(color clrCode,
                                         text_style txtCode) noexcept {
            style style;
            style.text = txtCode;
            style.set_color_(clrCode);
            return style;
        }

        inline constexpr style operator|(text_style txtCode,
                                         color clrCode) noexcept {
            return operator|(clrCode, txtCode);
        }

        inline constexpr style operator|(const style& othStyle,
                                         text_style txtCode) noexcept {
            style newStyle = othStyle;
            newStyle.text
                = static_cast<text_style>(std::to_underlying(othStyle.text)
                                          | std::to_underlying(txtCode));
            return newStyle;
        }

        inline constexpr style operator|(const style& othStyle,
                                         color clrCode) noexcept {
            style newStyle = othStyle;
            newStyle.set_color_(clrCode);
            return newStyle;
        }


        consteval std::string_view reset_seq() { return ANSI_CSI "0m"; }

        inline void reset(std::ostream& outstr = std::cout) {
            outstr << reset_seq();
        }


        inline std::string apply_seq(const style& style) {
            std::string seq;

            seq += to_ansi(style.fg);
            seq += to_ansi(style.bg);

            for (auto& i : detail::TEXT_STYLE_MAP) {
                if (std::to_underlying(i.key) & std::to_underlying(style.text))
                    seq += i.seq;
            }

            return seq;
        }

        inline void apply(const style& style,
                          std::ostream& outstr = std::cout) {
            outstr << to_ansi(style.fg);
            outstr << to_ansi(style.bg);

            for (auto& i : detail::TEXT_STYLE_MAP) {
                if (std::to_underlying(i.key) & std::to_underlying(style.text))
                    outstr << i.seq;
            }
        }

    } // namespace styles
} // namespace logging

#undef ANSI_CSI
#undef ANSI_ESC
