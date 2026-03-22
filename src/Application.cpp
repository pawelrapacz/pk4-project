#include "Application.h"

#include <raylib.h>

#include <iostream>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

using namespace Battleships;

Application::Application(const ApplicationSpecs& specs)
    : _specs(specs) { }

void Application::Run() {
    std::clog << _specs.width << std::endl << _specs.height << std::endl;
    InitWindow(_specs.width * _specs.scale, _specs.height * _specs.scale, _specs.title);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
}

void Application::UpdateDrawFrame() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Battleships!", 500, 300, 40, LIGHTGRAY);

    EndDrawing();
}