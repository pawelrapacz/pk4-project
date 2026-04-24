#pragma once

#include <array>
#include <cstdint>
#include <memory>

namespace Battleships {
    
    /// \enum Square
    /// \brief Represents a square on Player Grid
    /// \see Player::Grid
    enum class Square : std::uint8_t {
        None,
        Ship,
        Hit,
        Missed,
    };
    
    class Player {
    public:
        struct Pos { std::size_t x, y; };
        static constexpr std::size_t GRID_SIZE = 10;

        /// \brief Represents a game board, for marking shots, ships etc.
        /// \see Square
        using Grid = std::array<std::array<Square, GRID_SIZE>, GRID_SIZE>;

    public:
        static Grid RemoveShips(const Grid&) noexcept;
    
        Player();
        virtual ~Player() = default;
        
        bool Attack(Pos) noexcept;
        bool Attack(std::size_t x, std::size_t y) noexcept;
        const Grid& GetGrid() const noexcept;
        std::uint32_t GetHits() const noexcept;
        bool HasLost() const noexcept;
        
    protected:
        using ShipSize = std::uint32_t;
        
        struct ShipData {
            const ShipSize size;
            Pos start, end;
            ShipSize remainingSize = size;
        };
        using ShipDataGrid = std::array<std::array<std::shared_ptr<ShipData>, GRID_SIZE>, GRID_SIZE>;
    
    protected:
        static void InsertShipMargin(Grid&, const ShipData&) noexcept;

        ShipData& GetShip(std::size_t x, std::size_t y);
        const ShipData& GetShip(std::size_t x, std::size_t y) const;

    private:
        Player(Grid, ShipDataGrid);
    
    private:
        constexpr inline static std::uint32_t MAX_HITS = 20;
        
        Grid _grid = {};
        ShipDataGrid _shipData = {};
        std::uint32_t _hits = 0u;

        friend class PlayerBuilder;
    };

}