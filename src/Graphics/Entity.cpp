#include "Entity.h"

using namespace Battleships;

Entity::Entity(int x, int y) noexcept
    : _x(x), _y(y) { }

void Entity::SetPosition(int x, int y) noexcept {
    _x = x;
    _y = y;
}

int Entity::GetX() const noexcept {
    return _x;
}

int Entity::GetY() const noexcept {
    return _y;
}