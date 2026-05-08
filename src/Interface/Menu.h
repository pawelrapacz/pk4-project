#pragma once

namespace Battleships {

    class Application;

    class Menu {
      public:
        Menu(Application& app) noexcept
            : _app(app) { };

        virtual ~Menu() = default;

        virtual void OnUpdate()            = 0;
        virtual void Draw() const noexcept = 0;

      protected:
        Application& _app;
    };

} // namespace Battleships
