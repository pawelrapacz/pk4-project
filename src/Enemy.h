#pragma once

#include "Player.h"
#include <optional>

namespace Battleships {

    class Enemy : public Player {
    public:
        Enemy() = default;
        virtual ~Enemy() = default;
        virtual Pos MakeTurn(const Grid&) const = 0;
    };

    class SimpleEnemy : public Enemy {
    public:
        struct Vec {int x, y;};
        Pos MakeTurn(const Grid&) const override;
    protected:
        std::optional<Pos> SearchPossible(const Grid& grid, Pos pos) const;
        std::optional<Pos> SmartMove(const Grid& grid) const;
        std::optional<Pos> CheckDirection(const Grid& grid, Pos pos, Vec dir) const;
    };

    class AIEnemy : public SimpleEnemy {
        Pos MakeTurn(const Grid&) const override;
    };

}