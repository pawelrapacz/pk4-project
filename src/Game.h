#pragma once

#include "Player.h"
#include "Enemy.h"

#include <cstdint>
#include <memory>


namespace Battleships {    

    class Game {
    public:
        enum class State {
            Playing,
            PlayerWon,
            EnemyWon,
        };

    public:
        Game(const Player&);

        void OnUpdate();
        void Draw() const;
        State GetState() const noexcept;
        uint32_t GetTurn() const noexcept;
    
    private:
        void PlayerTurn(uint32_t, uint32_t) noexcept;
        void EnemyTurn() noexcept;

    private:
        Player _plr;
        std::unique_ptr<Enemy> _enm;
        uint32_t _turn = 0u;
        State _state = State::Playing;
    };

}