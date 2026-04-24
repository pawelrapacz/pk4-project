#pragma once

#include "PlayerBuilder.h"
#include "Interface/Menu.h"
#include "Interface/GridView.h"
#include "Interface/Button.h"

namespace Battleships {
    class FleetSetup : public Menu {
    public:
        FleetSetup(Application&);

        void OnUpdate() override;
        void Draw() const noexcept override;

    private:
        GridView _ghnd;
        Button _generateBtn;
        Button _startBtn;

        PlayerBuilder _bld;
    };
}