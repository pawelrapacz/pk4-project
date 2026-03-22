#include "Button.h"

#include <raylib.h>

#include <filesystem>
#include <functional>

using namespace Battleships;

Button::Button(int x, int y, int width, int height, std::string_view text,
               std::function<void()> callback, int fontSize, Color buttonColor, Color textColor)
    : Entity(x, y),
      _width(width),
      _height(height),
      _text(text),
      _callback(callback),
      _fontSize(fontSize),
      _btnClr(buttonColor),
      _txtClr(textColor) { }

void Button::OnUpdate() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && CheckCollisionPointRec(GetMousePosition(), GetRect()))
        _callback();
}

void Button::Draw() const {
    auto textWidth = MeasureText(_text.data(), _fontSize);
    int textX      = _x + (_width - textWidth) / 2;
    int textY      = _y + (_height - _fontSize) / 2;

    DrawRectangle(_x, _y, _width, _height, _btnClr);
    DrawText(_text.data(), textX, textY, _fontSize, _txtClr);
}

Rectangle Button::GetRect() const noexcept {
    return Rectangle(_x, _y, _width, _height);
}

TexButton::TexButton(int x, int y, std::filesystem::path path, std::function<void()> callback,
                     float scale)
    : Entity(x, y), _callback(callback), _scale(scale) {
    _tex = LoadTexture(path.string().c_str());
    _tex.width *= scale;
    _tex.height *= scale;
}

TexButton::~TexButton() {
    UnloadTexture(_tex);
}

void TexButton::OnUpdate() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && CheckCollisionPointRec(GetMousePosition(), GetRect()))
        _callback();
}

void TexButton::Draw() const {
    DrawTexture(_tex, _x, _y, WHITE);
}

Rectangle TexButton::GetRect() const noexcept {
    return Rectangle(_x, _y, _tex.width, _tex.height);
}