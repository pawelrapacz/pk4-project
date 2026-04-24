#include "PlayerBuilder.h"

#include <logging/logging.h>

#include <random>
#include <exception>
#include <stdexcept>


using namespace Battleships;

// #################  FOR Player::GenerateGrid  #################

// for the use of functions below Square::Missed will be treated
//  as a reserved position (margin between ships)
// Square::Hit will remain unused

// global random
std::mt19937 mt(std::random_device{}());

Player PlayerBuilder::BuildRandom() {
    static PlayerBuilder bld;
    bld.GenerateGrid();
    return bld.Build();
}

void PlayerBuilder::GenerateGrid() {
    logging::info("Generating Grid");
    _grid = { };
    _shipData = { };

    try {
        RandomInsertShip(4);
        RandomInsertShip(3);
        RandomInsertShip(3);
        RandomInsertShip(2);
        RandomInsertShip(2);
        RandomInsertShip(2);
        RandomInsertShip(1);
        RandomInsertShip(1);
        RandomInsertShip(1);
        RandomInsertShip(1);
    } catch (...) {
        logging::error("Generating grid failed");
        std::throw_with_nested(std::runtime_error("Generating grid failed"));
    }


    // erase Square::Missed (margins for insertion)
    for (std::size_t i {}; i < Player::GRID_SIZE; i++)
        for (std::size_t j {}; j < Player::GRID_SIZE; j++)
            if (_grid[i][j] != Square::Ship)
                _grid[i][j] = Square::None;
    
    logging::info("Successfully generated a grid");
}

const Player::Grid& PlayerBuilder::GetGrid() const noexcept {
    return _grid;
}

bool PlayerBuilder::ToggleSquare(Player::Pos pos) noexcept {
    if (_grid[pos.x][pos.y] == Square::Ship)
        _grid[pos.x][pos.y] = Square::None;
    else
        _grid[pos.x][pos.y] = Square::Ship;
    
    return true;
}

Player PlayerBuilder::Build() const {
    return {_grid, _shipData};
}

void PlayerBuilder::RandomInsertShip(const ShipSize size) {
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
                if (_grid[i][j] != Square::None)
                    foundPlacement = false;

        // Fail-safe: prevent infinite loop if random placement keeps failing.
        if(++hits > 100u) { // unlikely
            logging::error("Insering a ship of size {} failed {} hits.", size, hits);
            throw std::runtime_error("Inserting ship failed.");
        }
    }
    
    // insert ship and its data
    for (std::size_t i = ship->start.x; i < ship->end.x; i++)
        for (std::size_t j= ship->start.y; j < ship->end.y; j++) {
            _grid[i][j] = Square::Ship;
            _shipData[i][j] = ship;
        }

    InsertShipMargin(*ship); // sets margins
    logging::info("Successfully inserted a ship of size {} ({} hits)", size, hits);
}

void PlayerBuilder::InsertShipMargin(const ShipData& shipData) noexcept {
    Player::InsertShipMargin(_grid, shipData);
}