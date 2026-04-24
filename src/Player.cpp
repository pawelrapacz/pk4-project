#include "Player.h"

#include "PlayerBuilder.h"

#include <logging/logging.h>

#include <algorithm>
#include <stdexcept>

using namespace Battleships;


Player::Grid Player::RemoveShips(const Player::Grid& grid) noexcept {
    Grid newGrid = grid;
    for (std::size_t i = 0; i < GRID_SIZE; i++)
        for (std::size_t j = 0; j < GRID_SIZE; j++)
            if (newGrid[i][j] == Square::Ship)
                newGrid[i][j] = Square::None;
    return newGrid;
}

Player::Player()
    : Player(PlayerBuilder::BuildRandom()) { }

Player::Player(Grid grid, ShipDataGrid shipData)
    : _grid(std::move(grid)), _shipData(std::move(shipData)) { }

bool Player::Attack(std::size_t x, std::size_t y) noexcept {
    assert(x < GRID_SIZE && y < GRID_SIZE && "x and y must be in bounds of grid");
    switch (_grid[x][y]) {
        case Square::None: // MISS
            _grid[x][y] = Square::Missed;
            logging::info("Attack {}{}, missed!", char(y + 'a'), x);
            break;
        case Square::Ship: // HIT
            _hits++;
            _grid[x][y] = Square::Hit;
            GetShip(x, y).remainingSize--;

            logging::info("Attack {}{}, successful!", char(y + 'a'), x);
            if (GetShip(x, y).remainingSize == 0) {
                logging::info("Ship destroyed!");
                InsertShipMargin(_grid, GetShip(x, y));
            }
            break;
        default:
            logging::info("Attack {}{}, is invalid, that position has been already hit!", char(y + 'a'), x);
            return false;
    }
    return true;
}

bool Player::Attack(Pos pos) noexcept {
    return Attack(pos.x, pos.y);
}

std::uint32_t Player::GetHits() const noexcept {
    return _hits;
}

bool Player::HasLost() const noexcept {
    return GetHits() == MAX_HITS;
}

const Player::Grid& Player::GetGrid() const noexcept {
    return _grid;
}

Player::ShipData& Player::GetShip(std::size_t x, std::size_t y) {
    return const_cast<ShipData&>(std::as_const(*this).GetShip(x, y));
}

const Player::ShipData& Player::GetShip(std::size_t x, std::size_t y) const {
    if (_shipData[x][y]) {
        return *_shipData[x][y];
    }
    else {
        constexpr auto msg = "Can not acces ship that does not exist.";
        logging::error(msg);
        throw std::runtime_error(msg);
    }
}

// normalizes the value to be in bounds of Grid
// i.e. if you do x -= 1; you might get uint32_t(-1) which is wrong
constexpr std::size_t nmlz(std::size_t val, std::size_t cmp = Player::GRID_SIZE) noexcept {
    if (val < cmp)
        return val;
    else
        return 0u;
}

void Player::InsertShipMargin(Grid& grid, const ShipData& ship) noexcept {
    std::size_t startX = nmlz(ship.start.x - 1);
    std::size_t startY = nmlz(ship.start.y - 1);
    // make sure end does not overflow
    std::size_t endX = std::min(ship.end.x + 1, GRID_SIZE);
    std::size_t endY = std::min(ship.end.y + 1, GRID_SIZE);

    for (std::size_t i = startX; i < endX; i++)
        for (std::size_t j= startY; j < endY; j++)
            if (grid[i][j] == Square::None)
                grid[i][j] = Square::Missed;
}
