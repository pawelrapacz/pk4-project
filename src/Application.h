#pragma once

#include <memory>

#include "Game.h"

namespace Battleships {

    struct ApplicationState {
        bool gameRunning = false;
    };

    class Application {
    public:
        Application() = default;
        ~Application() = default;

        void Run();
        
    protected:
        void Loop();
        void OnUpdate();
        void Draw() const;

    private:
        ApplicationState _state;
        std::unique_ptr<Game> _game;
    };

}  // namespace Battleships