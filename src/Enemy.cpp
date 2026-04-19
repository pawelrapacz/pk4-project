#include "Enemy.h"

#include <random>

using namespace Battleships;

extern std::mt19937 mt; // defined in Player.cpp
static std::uniform_int_distribution<std::size_t> dist(0, 9);

SimpleEnemy::Pos SimpleEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    return {dist(mt), dist(mt)};
}

AIEnemy::Pos AIEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    // should do AI stuff
    return {dist(mt), dist(mt)};
}