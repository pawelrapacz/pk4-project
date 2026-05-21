#pragma once

#include "Player.h"

namespace Battleships {

    class Enemy : public Player {
      public:
        Enemy() = default;
        Enemy(Player&&);

        virtual ~Enemy() = default;

        virtual Pos MakeTurn(const Grid&) const = 0;
    };

    class SimpleEnemy : public Enemy {
      public:
        static Pos MakeTurnImpl(const Grid&);

        SimpleEnemy() = default;
        SimpleEnemy(Player&&);

        virtual ~SimpleEnemy() = default;

        // same as Impl
        Pos MakeTurn(const Grid&) const override;
    };

    class AIEnemy : public Enemy {
      public:
        static Pos MakeTurnImpl(const Grid&);

        AIEnemy() = default;
        AIEnemy(Player&&);

        virtual ~AIEnemy() = default;

        // same as Impl
        Pos MakeTurn(const Grid&) const override;
    };

} // namespace Battleships
