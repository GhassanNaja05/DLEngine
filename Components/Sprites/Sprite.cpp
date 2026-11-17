#include "Sprite.h"

#include "../../DLCore/Engine/Engine.h"
#include "../../DLCore/Modules/SpriteManager/SpriteManager.h"
#include "../../GameObject/GameObject.h"
#include "../../DLCore/GastonMath.h"

float Sprite::Width() const {
    return texture_width_ * internal_scale_ / Engine::Instance().true_unit_size;
}

float Sprite::Height() const {
    return texture_height_ * internal_scale_ / Engine::Instance().true_unit_size;
}

void Sprite::SetImage(const std::string& image) {
    texture_ = SpriteManager::Instance().GetTexture(image);
    texture_width_ = static_cast<float>(texture_->width);
    texture_height_ = static_cast<float>(texture_->height);
    texture_half_width_ = texture_width_ / 2.0f;
    texture_half_height_ = texture_height_ / 2.0f;
}

float& Sprite::Scale() {
    return internal_scale_;
}

void Sprite::ScaleToWidth(const float width) {
    internal_scale_ = width / texture_width_ * Engine::Instance().true_unit_size;
}

void Sprite::ScaleToHeight(const float height) {
    internal_scale_ = height / texture_height_ * Engine::Instance().true_unit_size;
}

Color &Sprite::DrawColor() {
    return color_;
}

void Sprite::Draw() const {

    if (!texture_)
        return;

    const Vector2 position = Owner()->GlobalPosition() * Engine::Instance().true_unit_size;
    const float scale = Owner()->GlobalScale() * internal_scale_;
    const float rotation = Owner()->GlobalRotation();



    const auto source_rect = Rectangle {
        0.0f,
        0.0f,
        texture_width_,
        texture_height_,
    };

    const auto dest_origin = position - rotated({
        texture_half_width_,
        texture_half_height_
    }, rotation) * scale;

    const auto dest_rect = Rectangle {
        dest_origin.x,
        dest_origin.y,
        texture_width_ * scale,
        texture_height_ * scale
    };

    DrawTexturePro(
        *texture_,
        source_rect,
        dest_rect,
        Vector2{0.0f, 0.0f},
        rotation,
        color_
    );
}
