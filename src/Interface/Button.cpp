#include "Button.h"

#include <raylib.h>

#include <filesystem>
#include <functional>
#include "Graphics/Entity.h"

using namespace Battleships;


Button::Button(const Rectangle& rec, const std::string& text, std::function<void()> callback)
    : _rec(rec), _text(text), _callback(callback) {
    _textX = _rec.x + (_rec.width - MeasureText(_text.c_str(), _fontSize)) /2;
    _textY = _rec.y + (_rec.height - _fontSize) / 2;    
}

Button::Button(float x, float y, const std::string& text, std::function<void()> callback)
    : _rec({x, y, DEFAULT_WIDTH, DEFAULT_HEIGHT}), _text(text), _callback(callback) {
    _textX = _rec.x + (_rec.width - MeasureText(_text.c_str(), _fontSize)) /2;
    _textY = _rec.y + (_rec.height - _fontSize) / 2;
}

Button::Button(const Rectangle& rec, const std::string& text, int fontSize, Color primaryClr, Color secondaryClr, Color textColor)
    : _fontSize(fontSize), _clrPri(primaryClr), _clrSec(secondaryClr), _txtClr(textColor), _rec(rec), _text(text) {
    _textX = _rec.x + (_rec.width - MeasureText(_text.c_str(), _fontSize)) /2;
    _textY = _rec.y + (_rec.height - _fontSize) / 2;
}

void Button::OnUpdate() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && CheckCollisionPointRec(GetMousePosition(), _rec))
        _callback();
}

void Button::Draw() const noexcept {
    const bool hover = CheckCollisionPointRec(GetMousePosition(), _rec);
    DrawRectangleRec(_rec, hover ? _clrSec : _clrPri);
    DrawText(_text.c_str(), _textX, _textY, _fontSize, _txtClr);
}

void Button::SetCallback(std::function<void()> callback) noexcept {
    _callback = callback;
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

void TexButton::Draw() const noexcept {
    DrawTexture(_tex, _x, _y, WHITE);
}

Rectangle TexButton::GetRect() const noexcept {
    return Rectangle(_x, _y, _tex.width, _tex.height);
}