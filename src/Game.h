#pragma once

#include "Enemy.h"
#include "Interface/GridView.h"
#include "Player.h"

#include <cstdint>
#include <future>
#include <memory>


namespace Battleships {

    class Application;


    class Game {
      public:
        Game(Application&, std::unique_ptr<Enemy>&&, Player&& = {}, std::uint32_t = {});
        ~Game();

        void OnUpdate();
        void Draw() const;
        std::unique_ptr<Enemy> ReleaseEnemy();
        bool Playing() const noexcept;
        std::uint32_t GetTurn() const noexcept;
        const Player& GetPlayer() const noexcept;
        const Enemy& GetEnemy() const noexcept;

      private:
        void EnemyTurn(); // async
        // player turn doesn't have to be async
        void ResolvePlayerTurn(Pos);
        void ResolveEnemyTurn(Pos);

      private:
        Application& _app;

        GridView _pghnd, _eghnd;

        Player _plr;
        std::unique_ptr<Enemy> _enm;
        std::future<Pos> _enmAttackFuture;
        std::uint32_t _turn = 0u;
        bool _playing       = true;
    };

} // namespace Battleships
