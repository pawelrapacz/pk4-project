#include "Application.h"
#include "Interface/Config.h"

#include <raylib.h>
#include <logging/logging.h>


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

using namespace Battleships;

void Application::Run() {
    logging::info("Initializing App");

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    _game = std::make_unique<Game>(Player());

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(Loop(), 0, 1);
#else
    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        Loop();
    }
#endif

    logging::info("Closing App");
    CloseWindow();
}

void Application::Loop() {
    OnUpdate();
    Draw();
}

void Application::OnUpdate() {
    _game->OnUpdate();
}

void Application::Draw() const {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawFPS(10, 10);
    _game->Draw();

    EndDrawing();
}