#include "RectangleImage.h"
#include "../../DLCore/GastonMath.h"

#include "../../DLCore/Engine/Engine.h"
#include "../../GameObject/GameObject.h"

RectangleImage::RectangleImage() : Drawable(), unit_size_(Engine::Instance().true_unit_size) {  }

float& RectangleImage::Width() {
    return width_;
}

float& RectangleImage::Height() {
    return height_;
}

Color& RectangleImage::RectColor() {
    return color_;
}

void RectangleImage::Draw() const {

    const Vector2& scaled_global_position = Owner()->GlobalPosition() * unit_size_;
    const float scaled_global_scale = Owner()->GlobalScale() * unit_size_;
    const float scaled_global_rotation = Owner()->GlobalRotation();

    const float width_offset = width_ / 2.0f * scaled_global_scale;
    const float height_offset = height_ / 2.0f * scaled_global_scale;

    const auto rect_offset = rotated({ width_offset, height_offset }, scaled_global_rotation);

    const auto [rect_x, rect_y] = scaled_global_position - rect_offset;

    const auto rect = Rectangle {
        rect_x,
        rect_y,
        width_ * scaled_global_scale,
        height_ * scaled_global_scale
    };

    DrawRectanglePro(rect, { 0, 0 }, scaled_global_rotation, color_);
}
