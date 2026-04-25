#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
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
        using ShipSize = std::uint32_t;

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
        struct ShipData {
            ShipSize size;
            Pos start, end;
            ShipSize remainingSize = size;
        };
        using ShipDataGrid = std::array<std::array<std::shared_ptr<ShipData>, GRID_SIZE>, GRID_SIZE>;
    
    private:
        static void InsertShipMargin(Grid&, const ShipData&) noexcept;
        
        // normalizes the value to be in bounds of Grid
        // i.e. if you do x -= 1; you might get uint32_t(-1) which is wrong
        static constexpr std::size_t nmlz(std::size_t val, std::size_t cmp = Player::GRID_SIZE) noexcept {
            if (val < cmp)
                return val;
            else
                return 0u;
        }

        Player(Grid, ShipDataGrid);
        
        ShipData& GetShip(std::size_t x, std::size_t y);
        const ShipData& GetShip(std::size_t x, std::size_t y) const;
    
    private:
        constexpr inline static std::uint32_t MAX_HITS = 20;
        
        Grid _grid = {};
        ShipDataGrid _shipData = {};
        std::uint32_t _hits = 0u;

        friend class PlayerBuilder;
    };

}