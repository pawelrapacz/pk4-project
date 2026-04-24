#pragma once

#include "Player.h"
#include <concepts>

namespace Battleships {

    class Enemy : public Player {
    public:
        Enemy() = default;
        virtual ~Enemy() = default;
        virtual Pos MakeTurn(const Grid&) const = 0;
    };

    class SimpleEnemy : public Enemy {
        Pos MakeTurn(const Grid&) const noexcept override;
    };

    class AIEnemy : public Enemy {
        Pos MakeTurn(const Grid&) const override;
    };

}