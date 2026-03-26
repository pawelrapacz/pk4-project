#include <cstdint>
#include <random>
#include <memory>
#include <algorithm>

#include "Player.h"
#include "Game.h"
#include "Interface/Config.h"

using namespace Battleships;

// Player::Player() {
//     _grid = GenerateGrid();
// }

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


// Player::Grid Player::GenerateGrid() {
//     static std::mt19937 mt(std::random_device{}());
//     static std::uniform_int_distribution<std::size_t> dist(0z, GRID_SIZE);
    
//     Grid grid { };
//     auto remaining = Rules::MAX_HITS;

//     while (remaining)
//         if (TryAddingShip(grid, dist(mt), dist(mt)))
//             remaining--;

//     return grid;
// }

// inline static bool HasCornerNeighbours(const Player::Grid& grid, std::size_t x, std::size_t y) {
//     // top left
//     if ()
// }

// bool Player::TryAddingShip(Player::Grid& grid, std::size_t x, std::size_t y) {
//     // checks for 
//     for (std::size_t i = std::clamp(x - 1, 0uz, 10uz); i < std::min(x+3z, GRID_SIZE); i+=2) {
//         for (std::size_t j = std::clamp(y - 1, 0uz, 10uz); j < std::min(y+3z,GRID_SIZE); j+=2) {
//             if (grid[i][j] == Square::Ship)
//                 return false;
//         }
//     }


//     std::array<std::array<std::shared_ptr<uint8_t>, GRID_SIZE>, GRID_SIZE> groups { };
//     std::array<uint8_t, 4> shipCount { };

//     for (std::size_t i = 0; i < GRID_SIZE; i++) {
//         for (std::size_t j = 0; j < GRID_SIZE; j++) {
//             if (grid[i][j] != Square::Ship) continue;

//             if (i > 0 && groups[i - 1][j])
//                 groups[i][j] = groups[i - 1][j];
//             else if (j > 0 && groups[i][j - 1])
//                 groups[i][j] = groups[i][j - 1];
//             else
//                 groups[i][j] = std::make_shared<uint8_t>();

//             shipCount[*groups[i][j]-1]--;
//             shipCount[*groups[i][j]]++;
//             (*groups[i][j])++;
//         }
//     }



//     uint8_t nbrCountNew { };
//     std::shared_ptr<uint8_t> groupNew;
//     for (std::size_t i = std::clamp(x - 1, 0uz, 10uz); i < std::min(x+3z, GRID_SIZE); i++) {
//         for (std::size_t j = std::clamp(y - 1, 0uz, 10uz); j < std::min(y+3z,GRID_SIZE); j++) {
//             if (i != x && j != y && grid[i][j] == Square::Ship) {
//                 nbrCountNew++;
//                 groupNew = groups[i][j];
//             }
//         }
//     }

//     if (nbrCountNew == 0 && shipCount[0] < Rules::DESTROYER_COUNT) {
//         return true;
//     }

//     if (nbrCountNew == 1 && )
// }
