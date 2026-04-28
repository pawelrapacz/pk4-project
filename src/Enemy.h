#pragma once

#include "Player.h"

namespace Battleships {

    class Enemy : public Player {
    public:
        Enemy() = default;
        virtual ~Enemy() = default;
        
        virtual Pos MakeTurn(const Grid&) const = 0;
    };

    class SimpleEnemy : public Enemy {
    public:
        static Pos MakeTurnImpl(const Grid&);
        
        // same as Impl
        Pos MakeTurn(const Grid&) const override;
    };

    class AIEnemy : public Enemy {
    public:
        static Pos MakeTurnImpl(const Grid&);

        // same as Impl
        Pos MakeTurn(const Grid&) const override;
    };

}