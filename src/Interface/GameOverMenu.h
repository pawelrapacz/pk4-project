#include "Button.h"
#include "Menu.h"

namespace Battleships {
    class GameOverMenu final : public Menu {
      public:
        GameOverMenu(Application& app);
        virtual ~GameOverMenu() = default;

        void OnUpdate() override;
        void Draw() const noexcept override;

      private:
        const Rectangle _rec;
        Button _newBtn;      // new game button
        Button _mainMenuBtn; // close game
    };
} // namespace Battleships
