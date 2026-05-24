#pragma once

#include "Enemy.h"
#include "Game.h"
#include "Interface/Menu.h"
#include "Player.h"

#include <concepts>
#include <memory>

namespace Battleships {

    struct ApplicationState {
        bool playerWon = false;
        bool showMenu  = true;
        bool running   = true;
    };

    class Application {
      public:
        Application()  = default;
        ~Application() = default;

        void Run();

        void RestartGame();
        void EndGame() noexcept;

        void ResumeLastGame();

        template<typename Tp>
            requires std::derived_from<Tp, Enemy> && std::default_initializable<Tp>
        void StartNewGame(Player plr = {}) {
            _game          = std::make_unique<Game>(*this, std::make_unique<Tp>(), std::move(plr));
            state.showMenu = false;
        }

        template<typename Tp>
            requires std::derived_from<Tp, Menu>
        void SetMenu() {
            // deffered menu update to prevent use after free
            _nextMenu      = std::make_unique<Tp>(*this);
            state.showMenu = true;
        }

      public:
        ApplicationState state;

      protected:
        void Loop();
        void OnUpdate();
        void Draw() const noexcept;
        void OnExit();

      private:
        std::unique_ptr<Game> _game;
        std::unique_ptr<Menu> _menu;
        std::unique_ptr<Menu> _nextMenu;
    };

} // namespace Battleships
