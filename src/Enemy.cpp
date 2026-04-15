#include "Enemy.h"

#include <random>

using namespace Battleships;

static std::mt19937 mt(std::random_device{}());
static std::uniform_int_distribution<uint32_t> dist(0, 9);

SimpleEnemy::Pos SimpleEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    return {dist(mt), dist(mt)};
}

AIEnemy::Pos AIEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    // should do AI stuff
    return {dist(mt), dist(mt)};
}