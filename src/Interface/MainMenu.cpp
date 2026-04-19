#include "MainMenu.h"
#include "Application.h"
#include "Config.h"
#include "Enemy.h"
#include "Interface/Button.h"

#include <logging/logging.h>

using namespace Battleships;

MainMenu::MainMenu(Application& app)
    : Menu(app),
    _newBtn({(WINDOW_WIDTH - 300) / 2.f, 200, 300, 70}, "New Game"),
    _newAIBtn({(WINDOW_WIDTH - 300) / 2.f, 290, 300, 70}, "New Game (AI)"),
    _quitBtn({(WINDOW_WIDTH - 300) / 2.f, 380, 300, 70}, "Quit") {
    logging::info("Initializing MainMenu");
    _newBtn.SetCallback([this](){
        this->_app.StartNewGame<SimpleEnemy>();
    });
    _newAIBtn.SetCallback([this](){
        this->_app.StartNewGame<AIEnemy>();
    });
    _quitBtn.SetCallback([this]() {
        this->_app.state.running = false;
    });
}

void MainMenu::OnUpdate() {
    _newBtn.OnUpdate();
    _newAIBtn.OnUpdate();
    _quitBtn.OnUpdate();
}

void MainMenu::Draw() const noexcept {
    constexpr auto text = "Menu";
    int posX = (WINDOW_WIDTH - MeasureText(text, FONT_SIZE_H1)) / 2;
    int posY = 100;

    DrawText(text, posX, posY, FONT_SIZE_H1, Colors::text);

    _newBtn.Draw();
    _newAIBtn.Draw();
    _quitBtn.Draw();
}
