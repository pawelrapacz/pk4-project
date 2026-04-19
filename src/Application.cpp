#include "Application.h"
#include "Interface/Config.h"
#include "Interface/MainMenu.h"

#include <raylib.h>
#include <logging/logging.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include <memory>
#include <exception>

using namespace Battleships;

void Application::Run() {
    logging::set_file("Battleships.log");
    logging::to_file();

    logging::info("Initializing App");

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    SetMenu<MainMenu>();
    
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(Loop(), 0, 1);
#else
    SetTargetFPS(30);

    try {

        while (!WindowShouldClose() && state.running)
            Loop();

    } catch (const std::exception& e) {
        logging::fatal("Unexpected error occured: what(): {}", e.what());
    } catch (...) {
        logging::fatal("Unexpected error occured: unknown");
    }

#endif

    logging::info("Closing App");
    CloseWindow();
}

void Application::RestartGame() {
    auto enemy = _game->ReleaseEnemy();
    _game = std::make_unique<Game>(*this, std::move(enemy));
}

void Application::EndGame() noexcept {
    logging::info("Ending Game");
    _game.reset();
}


void Application::Loop() {
    OnUpdate();
    Draw();
}

void Application::OnUpdate() {
    if (_menu)
        _menu->OnUpdate();

    if (_game)
        _game->OnUpdate();
}

void Application::Draw() const noexcept {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawFPS(10, 10);

    // order is important because some menus overlay Game
    if (_game)
        _game->Draw();

    if (_menu)
        _menu->Draw();


    EndDrawing();
}