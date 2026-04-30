#include "GameOverMenu.h"

#include "Config.h"
#include "Application.h"
#include "MainMenu.h"
#include "Interface/Button.h"

#include <raylib.h>
#include <logging/logging.h>

using namespace Battleships;

GameOverMenu::GameOverMenu(Application& game)
    : Menu(game),
    _rec({0, (WINDOW_HEIGHT - 250) / 2.f, WINDOW_WIDTH, 250}),
    _newBtn(WINDOW_WIDTH / 2.f - 20 - Button::DEFAULT_WIDTH, _rec.y + 160, "Quick Game"),
    _mainMenuBtn(WINDOW_WIDTH / 2.f + 20, _rec.y + 160,"Main Menu")
{
    logging::info("Initializing GameOverMenu");

    _newBtn.SetCallback([this](){
        this->_app.RestartGame();
    });
    _mainMenuBtn.SetCallback([this](){
        this->_app.EndGame();
        this->_app.SetMenu<MainMenu>();
    });
}

void GameOverMenu::OnUpdate() {
    _newBtn.OnUpdate();
    _mainMenuBtn.OnUpdate();
}

void GameOverMenu::Draw() const noexcept {
    DrawRectangleRec(_rec, Colors::white_t);
    // DrawRectangleLinesEx(_rec, 5, Colors::blue);

    // game over text
    float posX = (WINDOW_WIDTH - MeasureText("GAME OVER!", FONT_SIZE_H1)) / 2.f;
    float posY = _rec.y + MARGIN_H1;
    DrawText("GAME OVER!", posX, posY, FONT_SIZE_H1, Colors::text);

    const char* msg;
    if(_app.state.playerWon)
        msg = "You win!";
    else
        msg = "You lose";

    posX = (WINDOW_WIDTH - MeasureText(msg, FONT_SIZE_H2)) / 2.f;
    posY += FONT_SIZE_H1 + MARGIN_H2;
    DrawText(msg, posX, posY, FONT_SIZE_H2, Colors::text);

    _newBtn.Draw();
    _mainMenuBtn.Draw();
}
