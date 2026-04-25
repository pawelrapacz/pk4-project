#pragma once

#include "Player.h"

#include <cstddef>
#include <array>
#include <optional>
#include <type_traits>

namespace Battleships {

    namespace Rules {
        struct ShipSpec {
            Player::ShipSize size;
            std::size_t count;
        };

        constexpr auto FLEET_DEFINITION = std::to_array<ShipSpec>({
            {4, 1}, // carrier
            {3, 2}, // battleship
            {2, 3}, // crusier
            {1, 4}, // destroyer
        });

        using FleetSpec = std::remove_cv_t<decltype(FLEET_DEFINITION)>;
    }

    class PlayerBuilder {
    public:
        enum class ShipOrientation : uint8_t {
            Horizontal,
            Vertical
        };

    public:
        static Player BuildRandom();

        PlayerBuilder() = default;
        ~PlayerBuilder() = default;

        void GenerateRandomGrid();
        void Clear() noexcept;
        std::optional<Player::ShipSize> GetNextShipToInsert() const noexcept;
        bool TryInsertShip(Player::Pos, ShipOrientation);
        void RemoveShip(Player::Pos);
        const Player::Grid& GetGrid() const noexcept;
        Player Build() const;
    
    private:        
        using ShipSize = Player::ShipSize;
        using Pos = Player::Pos;
        using Grid = Player::Grid;
        using ShipData = Player::ShipData;
        using ShipDataGrid = Player::ShipDataGrid;

    private:
        static ShipData CreateShip(Pos, ShipSize, ShipOrientation) noexcept;
        static Grid RemoveMargins(const Grid&) noexcept;

        void InsertShipMargin(const ShipData&) noexcept;

        void RandomInsertShip(const ShipSize);
        void InsertShip(const ShipData&);
        bool ValidateShipPlacement(const ShipData&) const noexcept;

    private:
        static constexpr auto GRID_SIZE = Player::GRID_SIZE;

        Rules::FleetSpec _fleet = Rules::FLEET_DEFINITION;
        Grid _grid = {};
        ShipDataGrid _shipData = {};
    };

}