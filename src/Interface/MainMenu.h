#include "Menu.h"
#include "Button.h"

namespace Battleships {
    class MainMenu final : public Menu {
    public:
        MainMenu(Application&);
        
        void OnUpdate() override;
        void Draw() const noexcept override;
    
    private:
        Button _quickBtn;
        Button _quickAIBtn;
        Button _newBtn;
        Button _quitBtn;
    };
}