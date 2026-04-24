#pragma once

#include "Interface/Config.h"
#include "Player.h"

#include <optional>

namespace Battleships {

    class GridView {
    public:
        GridView(float x, float y, const char* label, float size = GRID_SQUARE_WIDTH);

        std::optional<Player::Pos> GetClick() const noexcept;
        void Draw(const Player::Grid&) const noexcept;
        float GetWidth() const noexcept;
        float GetHeight() const noexcept;

    private:
        void DrawGridLabels() const noexcept;
        void DrawGrid(const Player::Grid&) const noexcept;

    private:
        static constexpr float RATIO = 0.7f;

    private:
        const float _size; // square size
        float _gx, _gy; // grid
        float _hx, _hy; // grid headers
        float _lx, _ly; // label
        const char* _label;
    };

}