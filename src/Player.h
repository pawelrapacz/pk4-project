#pragma once

#include <array>
#include <cstdint>

#include "Interface/Config.h"

namespace Battleships {

    /// \enum Square
    /// \brief Represents a square on Players Grid
    /// \see GameGrid
    enum class Square : uint8_t {
        None,
        Ship,
        Hit,
        Missed,
    };

    class Player {
    public:
        /// \brief Represents a game board, for marking shots, ships etc.
        /// \see Square
        using Grid = std::array<std::array<Square, GRID_SIZE>, GRID_SIZE>;

    public:
        Player();
        Player(const Grid&);
        
        bool Hit(std::size_t x, std::size_t y) noexcept;
        const Grid& GetGrid() const noexcept;
        uint32_t GetHits() const noexcept;
        bool HasLost() const noexcept;
        
        Grid static GenerateGrid();

    private:
        Grid _grid;
        uint32_t _hits = 0u;
    };

}