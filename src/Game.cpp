#include <raylib.h>
#include <cstddef>
#include <cstdlib>
#include "Interface/Config.h"

#include "Game.h"

using namespace Battleships;

Game::Game(const Player& p1, const Player& p2)
    : _p1(p1), _p2(p2) { }


void Game::OnUpdate() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (std::size_t i = 0; i < GRID_SIZE; i++) {
            for (std::size_t j=0; j < GRID_SIZE; j++) {
                Rectangle rec {(float)600+40+i*40, (float)50+40+OFFSET_H3+j*40, 40, 40};
                if (CheckCollisionPointRec(GetMousePosition(), rec)) {
                    _p1.Hit(i, j);
                    return;
                }
            }
        }
    }
}

inline void DrawGridLabels(int x, int y, float size = GRID_SQUARE_WIDTH) {
    constexpr std::array<const char*, GRID_SIZE> LABEL_H = {"a", "b", "c", "d", "e", "f","g", "h", "i", "j"};
    constexpr std::array<const char*, GRID_SIZE> LABEL_V = {"1", "2", "3", "4", "5", "6","7", "8", "9", "10"};

    Rectangle rec {(float)x, (float)y, size, size};

    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        auto textPosY = (size - FONT_SIZE) / 2;
        
        // Horizontal Labels
        rec.x = x + size + i*size;
        rec.y = y;
        auto textPosXH = (size - MeasureText(LABEL_H[i], FONT_SIZE))/2;
        DrawRectangleRec(rec, Color(100,100,255,255));
        DrawRectangleLinesEx(rec, 1, WHITE);
        DrawText(LABEL_H[i], rec.x+textPosXH, rec.y+textPosY, FONT_SIZE, WHITE);
        
        // Vertial Labels
        rec.x = x;
        rec.y = y+size+i*size;        
        auto textPosXV = (size - MeasureText(LABEL_V[i], FONT_SIZE))/2;
        DrawRectangleRec(rec, Color(100,100,255,255));
        DrawRectangleLinesEx(rec, 1, WHITE);
        DrawText(LABEL_V[i], rec.x+textPosXV, rec.y+textPosY, FONT_SIZE, WHITE);
    }
}

inline void DrawGrid(int x, int y, const char* label, float size = GRID_SQUARE_WIDTH) {
    int labelPos = x+(11*size-MeasureText(label, FONT_SIZE_H3))/2;
    DrawText(label, labelPos, y, FONT_SIZE_H3, GRAY);
    y+=OFFSET_H3;

    DrawGridLabels(x, y, size);
    x+=size;
    y+=size;

    // draw grid
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            Rectangle rec {i*size+x, j*size+y, size, size};
            DrawRectangleRec(rec, Color(210,210,230,255));
            DrawRectangleLinesEx(rec, 1.3f, WHITE);
        }
    }
}

inline void DrawPlayerGridOverlay(const Player::Grid& grid, int x, int y, float size = GRID_SQUARE_WIDTH, float ratio = 0.7f) {
    float elSize = size * ratio;
    float elOffset = (size-elSize)/2;
    x+=size; y+=size+OFFSET_H3;

    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            Rectangle rec {x+i*size+elOffset, y+j*size+elOffset, elSize, elSize};
            switch (grid[i][j]) {
                case Square::Ship:
                    DrawRectangleRec(rec, GRAY);
                    break;
                case Square::Hit:
                    DrawRectangleRec(rec, RED);
                    break;
                case Square::Missed:
                    DrawRectangleRec(rec, BLUE);
                    break;
                default:
                    break;
            }
        }
    }
}

inline void DrawEnemyGridOverlay(const Player::Grid& grid, int x, int y, float size = GRID_SQUARE_WIDTH, float ratio = 0.7f) {
    float elSize = size * ratio;
    float elOffset = (size-elSize)/2;
    x+=size; y+=size+OFFSET_H3;

    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            Rectangle rec {x+i*size+elOffset, y+j*size+elOffset, elSize, elSize};
            switch (grid[i][j]) {
                case Square::Hit:
                    DrawRectangleRec(rec, RED);
                    break;
                case Square::Missed:
                    DrawRectangleRec(rec, BLUE);
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::Draw() const {
    DrawGrid(50, 50, "Player");
    DrawGrid(600, 50, "Enemy");
    DrawPlayerGridOverlay(_p1.GetGrid(), 50, 50);
    DrawEnemyGridOverlay(_p1.GetGrid(), 600, 50);
}


bool Game::Hit(std::size_t x, std::size_t y) noexcept {
    bool isGood;
    if (_turn % 2 == 0) {
        isGood = _p1.Hit(x, y);
        if (_p1.HasLost()) _state = State::Player2Won;
    }
    else {
        isGood = _p2.Hit(x, y);
        if (_p2.HasLost()) _state = State::Player1Won;
    }

    if (isGood) _turn++;
    return isGood;
}

Game::State Game::GetState() const noexcept {
    return _state;
}

uint32_t Game::GetTurn() const noexcept {
    return _turn % 2;
}