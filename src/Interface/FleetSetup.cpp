#include "Interface/FleetSetup.h"
#include "Application.h"


using namespace Battleships;

FleetSetup::FleetSetup(Application& app)
    : Menu(app),
      _ghnd(300, 50, "Arrange your fleet"),
      _generateBtn({750, 200, 300, 70}, "Generate Random", {}),
      _startBtn({750, 290, 300, 70}, "Start", {}) {
        _generateBtn.SetCallback([this](){
            this->_bld.GenerateGrid();
        });
      }


void FleetSetup::OnUpdate() {
    _generateBtn.OnUpdate();
    _startBtn.OnUpdate();
    
    if (auto pos = _ghnd.GetClick())
        _bld.ToggleSquare(pos.value());
}

void FleetSetup::Draw() const noexcept {
    _ghnd.Draw(_bld.GetGrid());
    _generateBtn.Draw();
    _startBtn.Draw();
}
