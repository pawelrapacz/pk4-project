#pragma once

namespace Battleships {

    class Entity {
      public:
        Entity() noexcept = default;
        Entity(int x, int y) noexcept;
        virtual ~Entity() = default;
        virtual void OnUpdate() { };
        virtual void Draw() const noexcept { };
        void SetPosition(int x, int y) noexcept;
        int GetX() const noexcept;
        int GetY() const noexcept;

      protected:
        int _x = 0;
        int _y = 0;
    };

} // namespace Battleships
