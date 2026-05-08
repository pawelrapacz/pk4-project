#pragma once

#include "Player.h"

#include <optional>

namespace Battleships {

    class GridView {
      public:
        static constexpr int GRID_SQUARE_WIDTH = 40;
        static constexpr int GRID_WIDTH        = GRID_SIZE * GRID_SQUARE_WIDTH;
        static constexpr int GRID_WIDTH_EXTRA  = GRID_WIDTH + GRID_SQUARE_WIDTH;

      public:
        GridView(float x, float y, const char* label,
                 float size = GRID_SQUARE_WIDTH);

        std::optional<Pos> GetClick() const noexcept;
        void Draw(const Grid&) const noexcept;
        float GetWidth() const noexcept;
        float GetHeight() const noexcept;

      private:
        void DrawGridLabels() const noexcept;
        void DrawGrid(const Grid&) const noexcept;

      private:
        static constexpr float RATIO = 0.7f;

      private:
        const float _size; // square size
        float _gx, _gy;    // grid
        float _hx, _hy;    // grid headers
        float _lx, _ly;    // label
        const char* _label;
    };

} // namespace Battleships
