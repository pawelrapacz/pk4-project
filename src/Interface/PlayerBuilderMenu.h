#pragma once

#include "Player.h"
#include "PlayerBuilder.h"
#include "Interface/Menu.h"
#include "Interface/GridView.h"
#include "Interface/Button.h"

#include <raylib.h>

namespace Battleships {
    class PlayerBuilderMenu : public Menu {
    public:
        PlayerBuilderMenu(Application&);

        void OnUpdate() override;
        void Draw() const noexcept override;

    private:
        void DrawShip(ShipSize) const noexcept;

    private:
        GridView _ghnd;
        Button _clearBtn;
        Button _generateBtn;
        Button _startBtn;
        Button _startAIBtn;

        Rectangle _nextShipView;

        PlayerBuilder::ShipOrientation _ort = PlayerBuilder::ShipOrientation::Vertical;
        PlayerBuilder _bld;
    };
}