#pragma once

#include "Interface/Config.h"

#include <array>
#include <cstdint>

namespace Battleships {

    /// \brief Contanins rules for arranging ships by the player
    namespace Rules {
        /// \defgroup ShipRules Ship Rules
        /// \brief Sets the rules for using and creating ships
        ///
        /// *_SIZE refers to the size of the Ship (in \ref Square Squares)\n
        /// *_COUNT is the number of ships given to the player at the start
        /// \see Square GameGrid PlayerBoard
        /// \{
        constexpr uint32_t CARRIER_SIZE = 4u;
        constexpr uint32_t BATTLESHIP_SIZE = 3u;
        constexpr uint32_t CRUSIER_SIZE = 2u;
        constexpr uint32_t DESTROYER_SIZE = 1u;

        constexpr uint32_t CARRIER_COUNT = 1u;
        constexpr uint32_t BATTLESHIP_COUNT = 2u;
        constexpr uint32_t CRUSIER_COUNT = 3u;
        constexpr uint32_t DESTROYER_COUNT = 4u;
        /// \}

        constexpr uint32_t MAX_HITS = CARRIER_SIZE * CARRIER_COUNT + BATTLESHIP_SIZE * BATTLESHIP_COUNT + CRUSIER_SIZE * CRUSIER_COUNT + DESTROYER_SIZE * DESTROYER_COUNT;
    }

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

        struct Pos { uint32_t x, y; };
    
    public:
        static Grid GenerateGrid();
        static Grid RemoveShips(const Grid&) noexcept;

    public:
        Player();
        Player(const Grid&);
        virtual ~Player() = default;
        
        bool Attack(Pos) noexcept;
        bool Attack(uint32_t x, uint32_t y) noexcept;
        const Grid& GetGrid() const noexcept;
        uint32_t GetHits() const noexcept;
        bool HasLost() const noexcept;
        
    private:
        Grid _grid;
        uint32_t _hits = 0u;
    };

}