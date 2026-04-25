#include "Application.h"
#include "Enemy.h"
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
#ifndef DEBUG // remove in Release
    SetTraceLogLevel(LOG_NONE);
#endif 
    // logging removers below functions in Release
    // with the LOGGING_DISABLE macro (see CMakeLists.txt)
    logging::to_file();
    // logging::set_file("Battleships.log");

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

    // create new enemy of the same type
    if (dynamic_cast<SimpleEnemy*>(enemy.get()))
        enemy = std::make_unique<SimpleEnemy>();
    else
        enemy = std::make_unique<AIEnemy>();

    // create new game
    _game = std::make_unique<Game>(*this, std::move(enemy));
    state.showMenu = false;
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
    // update menu (potentially change menu, or start game)
    if (_menu && state.showMenu)
        _menu->OnUpdate();

    // update game (potentially change menu)
    if (_game)
        _game->OnUpdate();

    // check for menu updates (order fixes flickering)
    if(_nextMenu)
        _menu = std::move(_nextMenu);
}

void Application::Draw() const noexcept {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    
#ifdef DEBUG
    DrawFPS(10, 10);
#endif

    // order is important because some menus overlay Game
    if (_game)
        _game->Draw();

    if (_menu && state.showMenu)
        _menu->Draw();


    EndDrawing();
}