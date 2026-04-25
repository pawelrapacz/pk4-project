#include "GridView.h"
#include "Interface/Config.h"
#include "Player.h"

#include <raylib.h>

#include <array>
#include <optional>

using namespace Battleships;


GridView::GridView(float x, float y, const char* label, float size)
    : _size(size), _label(label) {
    
    _lx = x + (_size * (GRID_SIZE + 1) - MeasureText(_label, FONT_SIZE_H3))/2;
    _ly = y;

    _hx = x;
    _hy = _ly + OFFSET_H3;

    _gx = _hx + GRID_SQUARE_WIDTH;
    _gy = _hy + GRID_SQUARE_WIDTH;
}


std::optional<Player::Pos> GridView::GetClick() const noexcept {
    // check click
    if (not IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return std::nullopt;
    
    // check colision
    Rectangle rec {_gx, _gy, _size, _size};
    auto mousePos = GetMousePosition();
    
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j=0; j < GRID_SIZE; j++) {
            if (CheckCollisionPointRec(mousePos, rec))
                return {{i ,j}}; // click on grid

            rec.y += _size;
        }
        rec.x += _size;
        rec.y = _gy;
    }

    // return on click not on grid
    return std::nullopt;
}


void GridView::Draw(const Player::Grid& grid) const noexcept {
    DrawText(_label, _lx, _ly, FONT_SIZE_H3, Colors::text);
    DrawGridLabels();
    DrawGrid(grid);
}

float GridView::GetWidth() const noexcept {
    return _size * (GRID_SIZE + 1);
}

float GridView::GetHeight() const noexcept {
    return GetWidth() + FONT_SIZE_H3;
}



void GridView::DrawGridLabels() const noexcept {
    constexpr auto LABEL_H = std::to_array({"a", "b", "c", "d", "e", "f","g", "h", "i", "j"});
    constexpr auto LABEL_V = std::to_array({"1", "2", "3", "4", "5", "6","7", "8", "9", "10"});

    // leave left top corner empty
    Rectangle recH {_hx + _size, _hy, _size, _size};
    Rectangle recV {_hx, _hy + _size, _size, _size};
    float textOffsetTop = (_size - FONT_SIZE) / 2;

    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        
        auto textPosXH = (_size - MeasureText(LABEL_H[i], FONT_SIZE))/2;
        DrawRectangleRec(recH, Colors::blue);
        DrawRectangleLinesEx(recH, 1, Colors::white);
        DrawText(LABEL_H[i], recH.x+textPosXH, recH.y+textOffsetTop, FONT_SIZE, Colors::white);
        
        // Vertial Labels
        auto textPosXV = (_size - MeasureText(LABEL_V[i], FONT_SIZE))/2;
        DrawRectangleRec(recV, Colors::blue);
        DrawRectangleLinesEx(recV, 1, Colors::white);
        DrawText(LABEL_V[i], recV.x+textPosXV, recV.y+textOffsetTop, FONT_SIZE, Colors::white);

        recH.x += _size;
        recV.y += _size;
    }
}

void GridView::DrawGrid(const Player::Grid& grid) const noexcept {
    float innerSize = _size * 0.7f;
    float innerOffset = (_size-innerSize)/2;

    Rectangle outer {_gx, _gy, _size, _size};
    Rectangle inner {_gx + innerOffset, _gy + innerOffset, innerSize, innerSize};
    
    // draw grid
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            DrawRectangleRec(outer, Colors::lightgray);
            DrawRectangleLinesEx(outer, 1.3f, Colors::white);

            switch (grid[i][j]) {
                case Square::Ship:
                    DrawRectangleRec(inner, Colors::gray);
                    break;
                case Square::Hit:
                    DrawRectangleRec(inner, Colors::red);
                    break;
                case Square::Missed:
                    // DrawRectangleRec(inner, Colors::lightblue);
                    DrawCircle(outer.x + outer.width / 2, outer.y + outer.height / 2, 3, Colors::black);
                    break;
                default:
                    break;
            }

            outer.y += _size;
            inner.y += _size;
        }
        outer.x += _size;
        outer.y = _gy;
        inner.x += _size;
        inner.y = _gy + innerOffset;
    }
}