#pragma once

#include <cstdint>

#include "Player.h"

namespace Battleships {

    /// \brief Contanins rules for arranging ships by the player
    namespace Rules {
        /// \defgroup ShipRules Ship Rules
        /// \brief Sets the rules for using and creating ships
        ///
        /// *_SIZE refers to the size of the Ship (in \ref Square Squares)\n
        /// *_COUNT is the number of ships given to the player at the start
        /// \see Square GameGrid PlayerBoard
        /// \{
        constexpr uint32_t CARRIER_SIZE = 4u;
        constexpr uint32_t BATTLESHIP_SIZE = 3u;
        constexpr uint32_t CRUSIER_SIZE = 2u;
        constexpr uint32_t DESTROYER_SIZE = 1u;

        constexpr uint32_t CARRIER_COUNT = 1u;
        constexpr uint32_t BATTLESHIP_COUNT = 2u;
        constexpr uint32_t CRUSIER_COUNT = 3u;
        constexpr uint32_t DESTROYER_COUNT = 4u;
        /// \}

        constexpr uint32_t MAX_HITS = CARRIER_SIZE * CARRIER_COUNT + BATTLESHIP_SIZE * BATTLESHIP_COUNT + CARRIER_SIZE * CARRIER_COUNT + DESTROYER_SIZE * DESTROYER_COUNT;
    }
    

    class Game {
    public:
        enum class State {
            Setup,
            Playing,
            Player1Won,
            Player2Won,
        };

    public:
        Game(const Player& p1, const Player& p2);

        void OnUpdate();
        void Draw() const;
        State GetState() const noexcept;
        uint32_t GetTurn() const noexcept;
    
    private:
        bool Hit(std::size_t x, std::size_t y) noexcept;
        void UpdateColor(std::size_t x, std::size_t y);

    private:
        Player _p1, _p2;
        uint32_t _turn = 0u;
        State _state = State::Setup;
    };
}