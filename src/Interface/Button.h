#pragma once

#include <raylib.h>

#include <filesystem>
#include <functional>
#include <string_view>

#include "Graphics/Entity.h"

namespace Battleships {

    class Button : public Entity {
    public:
        Button(int x, int y, int width, int height, std::string_view text,
               std::function<void()> callback, int fontSize = 20, Color buttonColor = LIGHTGRAY,
               Color textColor = BLACK);

        void OnUpdate() override;
        void Draw() const override;
        void SetCallback(std::function<void()>) noexcept;

    protected:
        Rectangle GetRect() const noexcept;

    private:
        int _width;
        int _height;
        std::string_view _text;
        std::function<void()> _callback;
        int _fontSize;
        Color _btnClr = LIGHTGRAY;
        Color _txtClr = BLACK;
    };

    class TexButton : public Entity {
    public:
        TexButton(int x, int y, std::filesystem::path path, std::function<void()> callback,
                  float scale = 1);
        virtual ~TexButton() override;

        void OnUpdate() override;
        void Draw() const override;
        void SetCallback(std::function<void()>) noexcept;

    protected:
        Rectangle GetRect() const noexcept;

    private:
        Texture2D _tex;
        std::function<void()> _callback;
        const float _scale;
    };

}  // namespace Battleships