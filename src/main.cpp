#include "Application.h"

#if defined(_WIN32) && !defined(DEBUG) // only Release builds
    #include <Windows.h>

    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
        Battleships::Application game;
        game.Run();
    }
#else
    int main() {
        Battleships::Application game;
        game.Run();
    }
#endif