#pragma once

#include "Player.h"

#include <array>

namespace Battleships {

    struct Vec {
        int x, y;
    };

    constexpr auto NeighbourVectors = std::to_array<Vec>({
        {-1,  0}, // top
        { 1,  0}, // bottom
        { 0, -1}, // left
        { 0,  1}, // right
    });

    inline constexpr bool InBounds(Pos pos, std::size_t cmp = GRID_SIZE) noexcept {
        // pos.x >= 0 and pos.y >= 0 always true (std::size_t)
        return pos.x < cmp && pos.y < cmp;
    }

    // normalizes the value to be in bounds of Grid
    // i.e. if you do x -= 1; you might get uint32_t(-1) which is wrong
    inline constexpr std::size_t Normalize(std::size_t val, std::size_t cmp = GRID_SIZE) noexcept {
        if (val < cmp)
            return val;
        else
            return 0u;
    }

} // namespace Battleships
