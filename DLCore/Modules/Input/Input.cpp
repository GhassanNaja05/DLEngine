#include "Input.h"

#include <raylib.h>
#include <unordered_map>

bool Input::IsKeyJustPressed(const KeyCode key) {
    const bool current = current_[key];
    const bool was_down = previous_[key];

    return current && !was_down;
}

bool Input::IsKeyJustReleased(const KeyCode key) {
    const bool current = current_[key];
    const bool was_down = previous_[key];

    return !current && was_down;
}

bool Input::IsKeyPressed(const KeyCode key) { return current_[key]; }

bool Input::IsMouseButtonJustPressed(const int button) {
    const bool current = current_mouse_[button];
    const bool was_down = previous_mouse_[button];

    return current && !was_down;
}

bool Input::IsMouseButtonJustReleased(const int button) {
    const bool current = current_mouse_[button];
    const bool was_down = previous_mouse_[button];

    return !current && was_down;
}

bool Input::IsMouseButtonPressed(const int button) { return current_mouse_[button]; }

void Input::Update(const float delta) {
    std::swap(previous_, current_);
    for (auto &[fst, snd] : current_) {
        snd = IsKeyDown(fst);
    }

    std::swap(previous_mouse_, current_mouse_);
    for (auto &[fst, snd] : current_mouse_) {
        snd = IsMouseButtonDown(fst);
    }

    Axes::UpdateAxes(delta);
}
