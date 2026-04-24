#pragma once

#include "Player.h"

namespace Battleships {

    class PlayerBuilder {
    public:
        static Player BuildRandom();

    public:
        void GenerateGrid();
        bool ToggleSquare(Player::Pos) noexcept;
        const Player::Grid& GetGrid() const noexcept;

        Player Build() const;
    
        
    private:
        void RandomInsertShip(const Player::ShipSize);
        void InsertShipMargin(const Player::ShipData&) noexcept;

    private:
        using ShipSize = Player::ShipSize;
        using Pos = Player::Pos;
        using Grid = Player::Grid;
        using ShipData = Player::ShipData;
        using ShipDataGrid = Player::ShipDataGrid;

    private:
        static constexpr auto GRID_SIZE = Player::GRID_SIZE;

        Grid _grid = {};
        ShipDataGrid _shipData = {};
    };

}