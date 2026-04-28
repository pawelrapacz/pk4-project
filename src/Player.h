#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <memory>
#include <type_traits>

namespace Battleships {

    using ShipSize = std::uint32_t;

    namespace Rules {

        struct ShipSpec {
            ShipSize size;
            std::size_t count;
        };

        constexpr auto FLEET_DEFINITION = std::to_array<ShipSpec>({
            {4, 1}, // carrier
            {3, 2}, // battleship
            {2, 3}, // crusier
            {1, 4}, // destroyer
        });

        consteval std::size_t MaxHits() noexcept {
            std::size_t maxHits = 0;
            for (auto i : FLEET_DEFINITION)
                maxHits += i.count * i.size;
            return maxHits;
        }
    
        constexpr auto MAX_HITS = MaxHits();
    
        using FleetSpec = std::remove_cv_t<decltype(FLEET_DEFINITION)>;
        using Hits = std::remove_cv_t<decltype(MAX_HITS)>;
    }

    /// \enum GridSquare
    /// \brief Represents a square on Player Grid
    /// \see Grid
    enum class GridSquare : std::uint8_t {
        None,
        Ship,
        Hit,
        Missed,
    };
    
    struct Pos { std::size_t x, y; };
    
    constexpr std::size_t GRID_SIZE = 10;

    /// \brief Represents a game board, for marking shots, ships etc.
    /// \see GridSquare
    using Grid = std::array<std::array<GridSquare, GRID_SIZE>, GRID_SIZE>;
    
    
    class Player {
    public:
        enum class AttackResult {
            InvalidPos,
            Missed,
            Hit,
            Destroyed,
        };

    public:
        static Grid RemoveShips(const Grid&) noexcept;
    
        Player();
        virtual ~Player() = default;
        
        AttackResult Attack(Pos) noexcept;
        AttackResult Attack(std::size_t x, std::size_t y) noexcept;
        const Grid& GetGrid() const noexcept;
        Rules::Hits GetHits() const noexcept;
        bool HasLost() const noexcept;
        

    private:
        struct ShipData {
            ShipSize size;
            Pos start, end;
            ShipSize remainingSize = size;
        };

        using ShipDataGrid = std::array<std::array<std::shared_ptr<ShipData>, GRID_SIZE>, GRID_SIZE>;
    
    private:
        // normalizes the value to be in bounds of Grid
        // i.e. if you do x -= 1; you might get uint32_t(-1) which is wrong
        static constexpr std::size_t nmlz(std::size_t val, std::size_t cmp = GRID_SIZE) noexcept {
            if (val < cmp)
                return val;
            else
                return 0u;
        }

        static void InsertShipMargin(Grid&, const ShipData&) noexcept;
        
        Player(Grid, ShipDataGrid); // for PlayerBuilder
        
        ShipData& GetShip(std::size_t x, std::size_t y);
        const ShipData& GetShip(std::size_t x, std::size_t y) const;
    
    private:
        Grid _grid = {};
        ShipDataGrid _shipData = {};
        Rules::Hits _hits = 0uz;

        friend class PlayerBuilder;
    };

}