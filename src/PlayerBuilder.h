#pragma once

#include "Player.h"

#include <optional>

namespace Battleships {

    class PlayerBuilder {
      public:
        enum class ShipOrientation { Horizontal, Vertical };

      public:
        static Player BuildRandom();
        static std::optional<Player> BuildFromGrid(const Grid&);

        PlayerBuilder()  = default;
        ~PlayerBuilder() = default;

        void GenerateRandomGrid();
        void Clear() noexcept;
        std::optional<ShipSize> GetNextShipToInsert() const noexcept;
        bool TryInsertShip(Pos, ShipOrientation);
        void RemoveShip(Pos);
        const Grid& GetGrid() const noexcept;
        bool Ready() const noexcept;
        Player Build() const;

      private:
        using ShipData     = Player::ShipData;
        using ShipDataGrid = Player::ShipDataGrid;

      private:
        static ShipData CreateShip(Pos, ShipSize, ShipOrientation) noexcept;
        static Grid RemoveMargins(const Grid&) noexcept;

        bool TryInsertShip(Pos, ShipSize, ShipOrientation);
        void InsertShipMargin(const ShipData&) noexcept;

        void RandomInsertShip(const ShipSize);
        void InsertShip(const ShipData&);
        bool ValidateShipPlacement(const ShipData&) const noexcept;

      private:
        Rules::FleetSpec _fleet = Rules::FLEET_DEFINITION;
        Grid _grid              = {};
        ShipDataGrid _shipData  = {};
    };

} // namespace Battleships
