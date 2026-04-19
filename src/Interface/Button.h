#pragma once

#include "Graphics/Entity.h"
#include "Config.h"

#include <raylib.h>

#include <filesystem>
#include <functional>
#include <string>

namespace Battleships {

    class Button {
    public:
        static constexpr float DEFAULT_FONT_SIZE = FONT_SIZE;
        static constexpr float DEFAULT_WIDTH = 200;
        static constexpr float DEFAULT_HEIGHT = 70;

    public:
        Button(const Rectangle& rec, const std::string& text, std::function<void()> callback = {});
        Button(float x, float y, const std::string& text, std::function<void()> callback = {});
        Button(const Rectangle& rec, const std::string& text, int fontSize, Color primaryClr, Color secondaryClr, Color textColor);

        void OnUpdate();
        void Draw() const noexcept;
        void SetCallback(std::function<void()>) noexcept;

    private:
        const int _fontSize = DEFAULT_FONT_SIZE;
        const Color _clrPri = Colors::blue;
        const Color _clrSec = Colors::lightblue;
        const Color _txtClr = Colors::white;

        Rectangle _rec;
        const std::string _text;
        std::function<void()> _callback;
        int _textX, _textY;
    };

    class TexButton : public Entity {
    public:
        TexButton(int x, int y, std::filesystem::path path, std::function<void()> callback,
                  float scale = 1);
        virtual ~TexButton() override;

        void OnUpdate() override;
        void Draw() const noexcept override;
        void SetCallback(std::function<void()>) noexcept;

    protected:
        Rectangle GetRect() const noexcept;

    private:
        Texture2D _tex;
        std::function<void()> _callback;
        const float _scale;
    };

}  // namespace Battleships