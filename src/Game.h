#pragma once

#include "Player.h"
#include "Enemy.h"

#include <cstdint>
#include <memory>


namespace Battleships {
    
    class Application;

    // enum class GameState {
    //     Playing,
    //     PlayerWon,
    //     EnemyWon,
    // };

    class Game {
    public:

    public:
        Game(Application&, std::unique_ptr<Enemy>, Player = {});

        void OnUpdate();
        void Draw() const;
        std::unique_ptr<Enemy> ReleaseEnemy();
        uint32_t GetTurn() const noexcept;
    
    private:
        void NextTurn();
        void PlayerTurn(std::size_t, std::size_t) noexcept;
        void EnemyTurn();

    private:
        Application& _app;

        Player _plr;
        std::unique_ptr<Enemy> _enm;
        uint32_t _turn = 0u;
        bool _playing = true;
    };

}