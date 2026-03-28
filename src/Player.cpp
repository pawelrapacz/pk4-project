#include <cstdint>
#include <random>
#include <algorithm>
#include <stdexcept>

#include "Player.h"
#include "Game.h"
#include "Interface/Config.h"

using namespace Battleships;

Player::Player()
    : _grid(GenerateGrid()) { }

Player::Player(const Grid& grid)
    : _grid(grid) { }

bool Player::Hit(std::size_t x, std::size_t y) noexcept {
    switch (_grid[x][y]) {
        case Square::None:
            _grid[x][y] = Square::Missed;
            break;
        case Square::Ship:
            _grid[x][y] = Square::Hit;
            _hits++;
            break;
        default:
            return false;
    }
    return true;
}

uint32_t Player::GetHits() const noexcept {
    return _hits;
}

bool Player::HasLost() const noexcept {
    return GetHits() == Rules::MAX_HITS;
}

const Player::Grid& Player::GetGrid() const noexcept {
    return _grid;
}



// #################  FOR Player::GenerateGrid  #################

// for the use of functions below Square::Hit will be treated
//  as a reserved position (margin between ships)
// Square::Missed will remain unused

// normalizes the value to be in bounds of grid
static inline auto nmlz(std::size_t val) {
    if (val < GRID_SIZE)
        return val;
    else
        return 0uz;
};

static void RandomInsertShip(Player::Grid& grid, std::size_t size) {
    static std::mt19937 mt(std::random_device{}());
    static std::uniform_int_distribution dir(0,1); // orientation/direction

    // unocnstrained
    static std::uniform_int_distribution<std::size_t> dist(0z, GRID_SIZE - 1);

    // constrained becoause of size
    std::uniform_int_distribution<std::size_t> cdist(0z, GRID_SIZE - size);

    uint32_t hits {};
    std::size_t x, y, mainCoord;
    int ort;
    bool foundCoor = false;

    while (!foundCoor) {
        foundCoor = true;
        ort = dir(mt);

        if (ort) { // horizontal
            mainCoord = x = cdist(mt);
            y = dist(mt);
        } else { // vertical
            x = dist(mt);
            mainCoord = y = cdist(mt);
        }   

        // checking if collides with other ships
        for (auto i = mainCoord; i < mainCoord + size; i++)
            if ((ort && grid[i][y] != Square::None) || (!ort && grid[x][i] != Square::None))
                foundCoor = false;

        if(++hits > 100u) // unlikely
            throw std::runtime_error("Generating game Grid failed.");
    }

    // sets margins, std::min fixes potential overflow
    if (ort) {
        for (auto i = nmlz(x - 1); i < std::min(GRID_SIZE, x + size + 1); i++)
            for (auto j= nmlz(y - 1); j < std::min(GRID_SIZE, y + 2); j++)
                grid[i][j] = Square::Hit;
    } else {
        for (auto i = nmlz(x - 1); i < std::min(GRID_SIZE, x + 2); i++)
            for (auto j= nmlz(y - 1); j < std::min(GRID_SIZE, y + size + 1); j++)
                grid[i][j] = Square::Hit;
    }

    // insert ship
    for (auto i = mainCoord; i < mainCoord + size; i++) {
        if (ort)
            grid[i][y] = Square::Ship; 
        else
            grid[x][i] = Square::Ship; 
    }   
}

Player::Grid Player::GenerateGrid() {
    Grid grid { };

    RandomInsertShip(grid, 4);
    RandomInsertShip(grid, 3);
    RandomInsertShip(grid, 3);
    RandomInsertShip(grid, 2);
    RandomInsertShip(grid, 2);
    RandomInsertShip(grid, 2);
    RandomInsertShip(grid, 1);
    RandomInsertShip(grid, 1);
    RandomInsertShip(grid, 1);
    RandomInsertShip(grid, 1);

    // erase Square::Hit (margins)
    for (std::size_t i {}; i < GRID_SIZE; i++)
        for (std::size_t j {}; j < GRID_SIZE; j++)
            if (grid[i][j] == Square::Hit)
                grid[i][j] = Square::None;

    return grid;
}