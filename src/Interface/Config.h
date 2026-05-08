#pragma once

#include <raylib.h>

namespace Battleships {
    constexpr auto TITLE = "Battleships";

    constexpr int WINDOW_WIDTH  = 1100;
    constexpr int WINDOW_HEIGHT = 600;

    constexpr int FONT_SIZE_H1 = 50;
    constexpr int FONT_SIZE_H2 = 40;
    constexpr int FONT_SIZE_H3 = 30;
    constexpr int FONT_SIZE    = 20;

    constexpr float MARGIN_RATIO = 0.5f;
    constexpr int MARGIN_H1      = FONT_SIZE_H1 * MARGIN_RATIO;
    constexpr int MARGIN_H2      = FONT_SIZE_H2 * MARGIN_RATIO;
    constexpr int MARGIN_H3      = FONT_SIZE_H3 * MARGIN_RATIO;
    constexpr int MARGIN_TEXT    = FONT_SIZE * MARGIN_RATIO;

    constexpr int OFFSET_H1   = FONT_SIZE_H1 + MARGIN_H1;
    constexpr int OFFSET_H2   = FONT_SIZE_H2 + MARGIN_H2;
    constexpr int OFFSET_H3   = FONT_SIZE_H3 + MARGIN_H3;
    constexpr int OFFSET_TEXT = FONT_SIZE + MARGIN_TEXT;

    namespace Colors {
        constexpr Color text      = {100, 100, 100, 255};
        constexpr Color white     = RAYWHITE;
        constexpr Color black     = BLACK;
        constexpr Color blue      = {100, 100, 255, 255};
        constexpr Color lightblue = {100, 170, 255, 255};
        constexpr Color red       = {255, 70, 70, 255};
        constexpr Color lightred  = {255, 100, 100, 255};
        constexpr Color gray      = GRAY;
        constexpr Color lightgray = Color(210, 210, 230, 255);


        constexpr Color make_transparent(Color clr,
                                         decltype(Color::a) a = 200) {
            return {clr.r, clr.g, clr.b, a};
        }

        constexpr Color white_t     = make_transparent(white);
        constexpr Color black_t     = make_transparent(black);
        constexpr Color blue_t      = make_transparent(blue);
        constexpr Color lightblue_t = make_transparent(lightblue);
        constexpr Color red_t       = make_transparent(red);
        constexpr Color lightred_t  = make_transparent(lightred);
        constexpr Color gray_t      = make_transparent(gray);
        constexpr Color lightgray_t = make_transparent(lightgray);

        constexpr Color ship   = gray;
        constexpr Color missed = black;
        constexpr Color hit    = red;
    } // namespace Colors
} // namespace Battleships
