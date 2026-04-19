#include "Game.h"
#include "Application.h"
#include "Interface/Config.h"
#include "Interface/GameOverMenu.h"
#include "Player.h"

#include <raylib.h>
#include <logging/logging.h>

#include <utility>
#include <memory>

using namespace Battleships;

Game::Game(Application& app, std::unique_ptr<Enemy> enm, Player plr)
    : _app(app), _plr(plr), _enm(std::move(enm)) { }

void Game::OnUpdate() {
    if (_playing) {
        NextTurn();
    }
}

inline void DrawGridLabels(int x, int y, float size = GRID_SQUARE_WIDTH) {
    constexpr auto LABEL_H = std::to_array({"a", "b", "c", "d", "e", "f","g", "h", "i", "j"});
    constexpr auto LABEL_V = std::to_array({"1", "2", "3", "4", "5", "6","7", "8", "9", "10"});

    Rectangle rec {(float)x, (float)y, size, size};

    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        auto textPosY = (size - FONT_SIZE) / 2;
        
        // Horizontal Labels
        rec.x = x + size + i*size;
        rec.y = y;
        auto textPosXH = (size - MeasureText(LABEL_H[i], FONT_SIZE))/2;
        DrawRectangleRec(rec, Colors::blue);
        DrawRectangleLinesEx(rec, 1, Colors::white);
        DrawText(LABEL_H[i], rec.x+textPosXH, rec.y+textPosY, FONT_SIZE, Colors::white);
        
        // Vertial Labels
        rec.x = x;
        rec.y = y+size+i*size;
        auto textPosXV = (size - MeasureText(LABEL_V[i], FONT_SIZE))/2;
        DrawRectangleRec(rec, Colors::blue);
        DrawRectangleLinesEx(rec, 1, Colors::white);
        DrawText(LABEL_V[i], rec.x+textPosXV, rec.y+textPosY, FONT_SIZE, Colors::white);
    }
}

inline void DrawGrid(int x, int y, const char* label, const Player::Grid& grid, float size = GRID_SQUARE_WIDTH, float sqRatio = 0.7f) {
    int labelPos = x+(11*size-MeasureText(label, FONT_SIZE_H3))/2;
    DrawText(label, labelPos, y, FONT_SIZE_H3, Colors::text);
    y+=OFFSET_H3;

    DrawGridLabels(x, y, size);
    x+=size;
    y+=size;

    float elSize = size * sqRatio;
    float elOffset = (size-elSize)/2;

    // draw grid
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            Rectangle outer {i*size+x, j*size+y, size, size};
            DrawRectangleRec(outer, Color(210,210,230,255));
            DrawRectangleLinesEx(outer, 1.3f, Colors::white);

            Rectangle inner {x+i*size+elOffset, y+j*size+elOffset, elSize, elSize};
            switch (grid[i][j]) {
                case Square::Ship:
                    DrawRectangleRec(inner, Colors::gray);
                    break;
                case Square::Hit:
                    DrawRectangleRec(inner, Colors::red);
                    break;
                case Square::Missed:
                    DrawRectangleRec(inner, Colors::lightblue);
                    break;
                default:
                    break;
            }
        }
    }
}


void Game::Draw() const {
    DrawGrid(50, 50, "Your fleet", _plr.GetGrid());
    DrawGrid(600, 50, "Enemy fleet", Player::RemoveShips(_enm->GetGrid()));
}

std::unique_ptr<Enemy> Game::ReleaseEnemy() {
    return std::move(_enm);
}

uint32_t Game::GetTurn() const noexcept {
    return _turn % 2;
}

void Game::NextTurn() {
    static uint32_t lastTurn {};

    if (lastTurn != GetTurn()) {
        logging::info("{}'s turn", GetTurn() ? "Enemy" : "Player");
        lastTurn = GetTurn();
    }

    if (GetTurn() == 0) {
        // check grid box click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (std::size_t i = 0; i < GRID_SIZE; i++) {
                for (std::size_t j=0; j < GRID_SIZE; j++) {
                    Rectangle rec {(float)600+40+i*40, (float)50+40+OFFSET_H3+j*40, 40, 40};
                    if (CheckCollisionPointRec(GetMousePosition(), rec)) {
                        PlayerTurn(i, j);
                    }
                }
            }
        }
    }
    else {
        EnemyTurn();
    }
}

void Game::PlayerTurn(std::size_t x, std::size_t y) noexcept {
    if (_enm->Attack(x, y))
        _turn++;

    if (_enm->HasLost()) {
        logging::info("Player won");
        _playing = false;
        _app.state.playerWon = true;
        _app.SetMenu<GameOverMenu>();
    }
}

void Game::EnemyTurn() {
    if (_plr.Attack(_enm->MakeTurn(_plr.GetGrid())))
        _turn++;

    if (_plr.HasLost()) {
        logging::info("Enemy won");
        _playing = false;
        _app.state.playerWon = false;
        _app.SetMenu<GameOverMenu>();
    }
}
