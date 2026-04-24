#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Interface/GridView.h"

#include <cstdint>
#include <memory>


namespace Battleships {
    
    class Application;


    class Game {
    public:
        Game(Application&, std::unique_ptr<Enemy>, Player = {});

        void OnUpdate();
        void Draw() const;
        std::unique_ptr<Enemy> ReleaseEnemy();
        std::uint32_t GetTurn() const noexcept;
    
    private:
        void NextTurn();
        void PlayerTurn(Player::Pos) noexcept;
        void EnemyTurn();

    private:
        Application& _app;
        
        GridView _pghnd, _eghnd;
        
        Player _plr;
        std::unique_ptr<Enemy> _enm;
        std::uint32_t _turn = 0u;
        bool _playing = true;
    };

}