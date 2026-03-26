#pragma once

namespace Battleships {
    constexpr auto TITLE = "Battleships";
    
    constexpr int WINDOW_WIDTH = 1100;
    constexpr int WINDOW_HEIGHT = 700;

    constexpr int FONT_SIZE_H1 = 50;
    constexpr int FONT_SIZE_H2 = 40;
    constexpr int FONT_SIZE_H3 = 30;
    constexpr int FONT_SIZE = 20;

    constexpr float MARGIN_RATIO = 0.5f;
    constexpr int MARGIN_H1 = FONT_SIZE_H1 * MARGIN_RATIO;
    constexpr int MARGIN_H2 = FONT_SIZE_H2 * MARGIN_RATIO;
    constexpr int MARGIN_H3 = FONT_SIZE_H3 * MARGIN_RATIO;
    constexpr int MARGIN_TEXT = FONT_SIZE * MARGIN_RATIO;

    constexpr auto GRID_SIZE = 10uz;
    constexpr int GRID_SQUARE_WIDTH = 40;
    constexpr int GRID_WIDTH = GRID_SIZE * GRID_SQUARE_WIDTH;
    constexpr int GRID_WIDTH_EXTRA = GRID_WIDTH + GRID_SQUARE_WIDTH;

    constexpr int OFFSET_H1 = FONT_SIZE_H1 + MARGIN_H1;
    constexpr int OFFSET_H2 = FONT_SIZE_H2 + MARGIN_H2;
    constexpr int OFFSET_H3 = FONT_SIZE_H3 + MARGIN_H3;
    constexpr int OFFSET_TEXT = FONT_SIZE + MARGIN_TEXT;
}