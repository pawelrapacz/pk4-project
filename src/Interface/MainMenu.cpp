#include "MainMenu.h"

#include "Application.h"
#include "Config.h"
#include "Enemy.h"
#include "Interface/Button.h"
#include "Interface/PlayerBuilderMenu.h"

#include <logging/logging.h>
#include <raylib.h>

using namespace Battleships;

MainMenu::MainMenu(Application& app)
    : Menu(app),
      _quickBtn({(WINDOW_WIDTH - 300) / 2.f, 200, 300, 70}, "Quick Game"),
      _quickAIBtn({(WINDOW_WIDTH - 300) / 2.f, 290, 300, 70},
                  "Quick Game (AI)"),
      _newBtn({(WINDOW_WIDTH - 300) / 2.f, 380, 300, 70}, "New Game"),
      _quitBtn({(WINDOW_WIDTH - 300) / 2.f, 470, 300, 70}, "Quit") {
    logging::info("Initializing MainMenu");
    _quickBtn.SetCallback([this]() { this->_app.StartNewGame<SimpleEnemy>(); });
    _quickAIBtn.SetCallback([this]() { this->_app.StartNewGame<AIEnemy>(); });
    _newBtn.SetCallback([this]() { this->_app.SetMenu<PlayerBuilderMenu>(); });
    _quitBtn.SetCallback([this]() { this->_app.state.running = false; });
}

void MainMenu::OnUpdate() {
    _quickBtn.OnUpdate();
    _quickAIBtn.OnUpdate();
    _newBtn.OnUpdate();
    _quitBtn.OnUpdate();
}

void MainMenu::Draw() const noexcept {
    constexpr auto text = "Menu";
    int posX            = (WINDOW_WIDTH - MeasureText(text, FONT_SIZE_H1)) / 2;
    int posY            = 100;

    DrawText(text, posX, posY, FONT_SIZE_H1, Colors::text);

    _quickBtn.Draw();
    _quickAIBtn.Draw();
    _newBtn.Draw();
    _quitBtn.Draw();
}
