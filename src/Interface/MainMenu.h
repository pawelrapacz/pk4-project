#include "Button.h"
#include "Menu.h"

namespace Battleships {
    class MainMenu final : public Menu {
      public:
        MainMenu(Application&);

        void OnUpdate() override;
        void Draw() const noexcept override;

      private:
        Button _quickBtn;
        Button _quickAIBtn;
        Button _resumeBtn;
        Button _newBtn;
        // Button _quitBtn;
    };
} // namespace Battleships
