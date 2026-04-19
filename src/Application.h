#pragma once

#include "Interface/Menu.h"
#include "Game.h"

#include <concepts>
#include <memory>

namespace Battleships {

    struct ApplicationState {
        bool playerWon = false;
        bool running = true;
    };

    class Application {
    public:
        Application() = default;
        ~Application() = default;

        void Run();

        void RestartGame();
        void EndGame() noexcept;

        template<typename Tp>
            requires std::derived_from<Tp, Enemy>
        void StartNewGame() {
            _game = std::make_unique<Game>(*this, std::make_unique<Tp>());
            _menu.reset();
        }

        template<typename Tp>
            requires std::derived_from<Tp, Menu>
        void SetMenu() {
            _menu = std::make_unique<Tp>(*this);
        }

    public:
        ApplicationState state;

    protected:
        void Loop();
        void OnUpdate();
        void Draw() const noexcept;

    private:
        std::unique_ptr<Game> _game;
        std::unique_ptr<Menu> _menu;
    };

}  // namespace Battleships