#include "Player.h"

#include "GridHelpers.h"
#include "PlayerBuilder.h"

#include <logging/logging.h>

#include <algorithm>
#include <stdexcept>

using namespace Battleships;


Grid Player::RemoveShips(const Grid& grid) noexcept {
    Grid newGrid = grid;
    for (std::size_t i = 0; i < GRID_SIZE; i++)
        for (std::size_t j = 0; j < GRID_SIZE; j++)
            if (newGrid[i][j] == GridSquare::Ship) newGrid[i][j] = GridSquare::None;
    return newGrid;
}

Player::Player()
    : Player(PlayerBuilder::BuildRandom()) { }

Player::Player(Grid grid, ShipDataGrid&& shipData)
    : _grid(std::move(grid)), _shipData(std::move(shipData)) { }

Player::AttackResult Player::Attack(std::size_t x, std::size_t y) noexcept {
    assert(x < GRID_SIZE && y < GRID_SIZE && "x and y must be in bounds of grid");

    AttackResult res {};
    switch (_grid[x][y]) {
    case GridSquare::None: // MISS
        _grid[x][y] = GridSquare::Missed;
        res         = AttackResult::Missed;
        logging::info("Attack {}{}, missed!", char(x + 'a'), y + 1);
        break;
    case GridSquare::Ship: // HIT
        _hits++;
        _grid[x][y] = GridSquare::Hit;
        GetShip(x, y).remainingSize--;
        res = AttackResult::Hit;

        logging::info("Attack {}{}, successful!", char(x + 'a'), y + 1);
        // Check if ship is destroyed
        if (GetShip(x, y).remainingSize == 0) {
            logging::info("Ship destroyed!");
            InsertShipMargin(_grid, GetShip(x, y));
            res = AttackResult::Destroyed;
        }
        break;
    default: // invalid position
        logging::info("Attack {}{}, is invalid, that position has been already hit!", char(y + 'a'),
                      x);
        res = AttackResult::InvalidPos;
    }
    return res;
}

Player::AttackResult Player::Attack(Pos pos) noexcept { return Attack(pos.x, pos.y); }

Rules::Hits Player::GetHits() const noexcept { return _hits; }

bool Player::HasLost() const noexcept { return GetHits() == Rules::MAX_HITS; }

const Grid& Player::GetGrid() const noexcept { return _grid; }

Player::ShipData& Player::GetShip(std::size_t x, std::size_t y) {
    return const_cast<ShipData&>(std::as_const(*this).GetShip(x, y));
}

const Player::ShipData& Player::GetShip(std::size_t x, std::size_t y) const {
    if (_shipData[x][y]) {
        return *_shipData[x][y];
    } else {
        constexpr auto msg = "Can not acces ship that does not exist.";
        logging::error(msg);
        throw std::runtime_error(msg);
    }
}

void Player::InsertShipMargin(Grid& grid, const ShipData& ship) noexcept {
    std::size_t startX = Normalize(ship.start.x - 1);
    std::size_t startY = Normalize(ship.start.y - 1);
    // make sure end does not overflow
    std::size_t endX = std::min(ship.end.x + 1, GRID_SIZE);
    std::size_t endY = std::min(ship.end.y + 1, GRID_SIZE);

    for (std::size_t i = startX; i < endX; i++)
        for (std::size_t j = startY; j < endY; j++)
            if (grid[i][j] == GridSquare::None) grid[i][j] = GridSquare::Missed;
}
