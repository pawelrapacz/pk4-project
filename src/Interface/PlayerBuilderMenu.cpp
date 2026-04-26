#include "Interface/PlayerBuilderMenu.h"
#include "Application.h"
#include "Enemy.h"
#include "Interface/Config.h"
#include "Interface/GridView.h"
#include "Player.h"
#include "PlayerBuilder.h"
#include "raylib.h"


using namespace Battleships;

PlayerBuilderMenu::PlayerBuilderMenu(Application& app)
    : Menu(app),
      _ghnd(300, 50, "Arrange your fleet"),
      _clearBtn({770, 160, 300, 70}, "Clear", {}),
      _generateBtn({770, 250, 300, 70}, "Generate Random", {}),
      _startBtn({770, 340, 300, 70}, "Start Game", {}),
      _startAIBtn({770, 430, 300, 70}, "Start Game (AI)", {}),
      _nextShipView({50, 200, 200, 200}) {
        logging::info("Initializing PlayerBuilderMenu");
        _clearBtn.SetCallback([this]() {
            this->_bld.Clear();
        });
        _generateBtn.SetCallback([this]() {
            this->_bld.GenerateRandomGrid();
        });
        _startBtn.SetCallback([this]() {
            if (this->_bld.Ready())
                this->_app.StartNewGame<SimpleEnemy>(this->_bld.Build());
        });
        _startAIBtn.SetCallback([this]() {
            if (this->_bld.Ready())
                this->_app.StartNewGame<AIEnemy>(this->_bld.Build());
        });
      }


void PlayerBuilderMenu::OnUpdate() {
    _clearBtn.OnUpdate();
    _generateBtn.OnUpdate();
    _startBtn.OnUpdate();
    _startAIBtn.OnUpdate();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), _nextShipView)) {
        if (_ort == PlayerBuilder::ShipOrientation::Horizontal)
            _ort = PlayerBuilder::ShipOrientation::Vertical;
        else
            _ort = PlayerBuilder::ShipOrientation::Horizontal;
    }
    
    if (auto pos = _ghnd.GetClick()) {
        if (_bld.GetGrid()[pos.value().x][pos.value().y] == Square::Ship)
            _bld.RemoveShip(pos.value());
        else
            _bld.TryInsertShip(pos.value(), _ort);
    }
}

void PlayerBuilderMenu::Draw() const noexcept {
    DrawRectangleRec(_nextShipView, Colors::white);
    DrawRectangleLinesEx(_nextShipView, 1.7, Colors::black);

    if (auto size = _bld.GetNextShipToInsert()) {
        DrawShip(size.value()); 
    }

    _ghnd.Draw(_bld.GetGrid());
    _clearBtn.Draw();
    _generateBtn.Draw();
    _startBtn.Draw();
    _startAIBtn.Draw();
}


void PlayerBuilderMenu::DrawShip(Player::ShipSize size) const noexcept {
    float innerSize = GRID_SQUARE_WIDTH * 0.7f;
    float innerOffset = (GRID_SQUARE_WIDTH-innerSize)/2;

    if (_ort == PlayerBuilder::ShipOrientation::Horizontal) {
        float x = _nextShipView.x + (_nextShipView.width - GRID_SQUARE_WIDTH * size) / 2;
        float y = _nextShipView.y + (_nextShipView.height - GRID_SQUARE_WIDTH) / 2;
        Rectangle inner {x + innerOffset, y + innerOffset, innerSize, innerSize};
        for (Player::ShipSize i {}; i < size; i++) {
            DrawRectangleRec(inner, Colors::gray);
            inner.x += GRID_SQUARE_WIDTH;
        }
    } else {
        float x = _nextShipView.x + (_nextShipView.width - GRID_SQUARE_WIDTH) / 2;
        float y = _nextShipView.y + (_nextShipView.height - GRID_SQUARE_WIDTH * size) / 2;
        Rectangle inner {x + innerOffset, y + innerOffset, innerSize, innerSize};
        for (Player::ShipSize i {}; i < size; i++) {
            DrawRectangleRec(inner, Colors::gray);
            inner.y += GRID_SQUARE_WIDTH;
        }
    }
}