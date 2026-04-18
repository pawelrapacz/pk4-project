#include "Interface/Config.h"
#include "Player.h"

#include <logging/logging.h>

#include <memory>
#include <random>
#include <algorithm>
#include <stdexcept>

using namespace Battleships;

// global random
std::mt19937 mt(std::random_device{}());

Player::Grid Player::RemoveShips(const Player::Grid& grid) noexcept {
    Grid newGrid = grid;
    for (std::size_t i = 0; i < GRID_SIZE; i++)
        for (std::size_t j = 0; j < GRID_SIZE; j++)
            if (newGrid[i][j] == Square::Ship)
                newGrid[i][j] = Square::None;
    return newGrid;
}

Player::Player() {
    GenerateGrid(_grid, _shipData);
}


Player::Player(const Grid& grid)
    : _grid(grid) { }


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

uint32_t Player::GetHits() const noexcept {
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


// #################  FOR Player::GenerateGrid  #################

// for the use of functions below Square::Missed will be treated
//  as a reserved position (margin between ships)
// Square::Hit will remain unused

void Player::RandomInsertShip(Grid& grid, ShipDataGrid& dataGrid[[maybe_unused]], const ShipSize size) {
    static std::bernoulli_distribution direction(0.5); // orientation/direction
    // unocnstrained
    static std::uniform_int_distribution<std::size_t> udist(0z, GRID_SIZE - 1);
    // constrained because of size
    std::uniform_int_distribution<std::size_t> cdist(0z, GRID_SIZE - size);
    
    auto ship = std::make_shared<ShipData>(ShipData{size, {}, {}});
    uint32_t hits {};
    bool foundPlacement = false;

    // searching for possible ship placement
    while (!foundPlacement) {
        if (direction(mt)) { // horizontal
            ship->start.x = cdist(mt);
            ship->start.y = udist(mt);
            ship->end.x = ship->start.x + size;
            ship->end.y = ship->start.y + 1;
        } else { // vertical
            ship->start.x = udist(mt);
            ship->start.y = cdist(mt);
            ship->end.x = ship->start.x + 1;
            ship->end.y = ship->start.y + size;
        }
        
        // checking collision with other ships/margins
        // if it collides the placment is rejected
        foundPlacement = true;
        for (std::size_t i = ship->start.x; i < ship->end.x; i++)
            for (std::size_t j= ship->start.y; j < ship->end.y; j++)
                if (grid[i][j] != Square::None)
                    foundPlacement = false;

        // Fail-safe: prevent infinite loop if random placement keeps failing.
        if(++hits > 100u) { // unlikely
            logging::error("Generating game Grid failed {} hits", hits);
            throw std::runtime_error("Generating game Grid failed.");
        }
    }
    
    // insert ship and its data
    for (std::size_t i = ship->start.x; i < ship->end.x; i++)
        for (std::size_t j= ship->start.y; j < ship->end.y; j++) {
            grid[i][j] = Square::Ship;
            dataGrid[i][j] = ship;
        }

    InsertShipMargin(grid, *ship); // sets margins
    logging::info("Successfully inserted ship of size {} ({} hits)", size, hits);
}

void Player::GenerateGrid(Player::Grid& grid, Player::ShipDataGrid& dataGrid) {
    logging::info("Generating Grid");
    grid = { };
    dataGrid = { };

    RandomInsertShip(grid, dataGrid, 4);
    RandomInsertShip(grid, dataGrid, 3);
    RandomInsertShip(grid, dataGrid, 3);
    RandomInsertShip(grid, dataGrid, 2);
    RandomInsertShip(grid, dataGrid, 2);
    RandomInsertShip(grid, dataGrid, 2);
    RandomInsertShip(grid, dataGrid, 1);
    RandomInsertShip(grid, dataGrid, 1);
    RandomInsertShip(grid, dataGrid, 1);
    RandomInsertShip(grid, dataGrid, 1);

    // erase Square::Missed (margins for insertion)
    for (std::size_t i {}; i < GRID_SIZE; i++)
        for (std::size_t j {}; j < GRID_SIZE; j++)
            if (grid[i][j] != Square::Ship)
                grid[i][j] = Square::None;
    
    logging::info("Successfully generated a grid");
}


// normalizes the value to be in bounds of Grid
// i.e. if you do x -= 1; you might get uint32_t(-1) which is wrong
constexpr std::size_t nmlz(std::size_t val, std::size_t cmp = GRID_SIZE) noexcept {
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
