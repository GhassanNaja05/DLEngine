#include "TiledSprite.h"

#include "Sprite.h"

#include "../../DLCore/Engine/Engine.h"
#include "../../DLCore/Modules/SpriteManager/SpriteManager.h"
#include "../../GameObject/GameObject.h"
#include "../../DLCore/GastonMath.h"

float TiledSprite::Width() const {
    return width_;
}

float TiledSprite::Height() const {
    return height_;
}

void TiledSprite::SetWidth(const float width) {
    assert(width > 0.0f);
    width_ = width;
}

void TiledSprite::SetHeight(const float height) {
    assert(height > 0.0f);
    height_ = height;
}

float TiledSprite::GetWidth() const {
    return width_;
}

float TiledSprite::GetHeight() const {
    return height_;
}

void TiledSprite::SetImage(const std::string& image) {
    texture_ = SpriteManager::Instance().GetTexture(image);
    texture_width_ = static_cast<float>(texture_->width);
    texture_height_ = static_cast<float>(texture_->height);
    texture_half_width_ = texture_width_ / 2.0f;
    texture_half_height_ = texture_height_ / 2.0f;
}

float& TiledSprite::Scale() {
    return internal_scale_;
}

void TiledSprite::SetUnitWidth(const float width) {
    assert(width > 0.0f);
    internal_scale_ = width / texture_width_ * Engine::Instance().true_unit_size;
    unit_width_ = width;
    unit_height_ = texture_height_ / texture_width_ * width;
}

void TiledSprite::SetUnitHeight(const float height) {
    assert(height > 0.0f);
    internal_scale_ = height / texture_height_ * Engine::Instance().true_unit_size;
    unit_height_ = height;
    unit_width_ = texture_width_ / texture_height_ * height;
}

float TiledSprite::GetUnitWidth() const {
    return unit_width_;
}

float TiledSprite::GetUnitHeight() const {
    return unit_height_;
}

Vector2 TiledSprite::GetOffset() const {
    return offset_;
}

void TiledSprite::SetOffset(const Vector2 &offset) {

    offset_ = offset;

    offset_.x = std::fmod(offset_.x, unit_width_);
    if (offset_.x < 0.0f)
        offset_.x += unit_width_;

    offset_.y = std::fmod(offset_.y, unit_height_);
    if (offset_.y < 0.0f)
        offset_.y += unit_height_;
}
Color &TiledSprite::DrawColor() {
    return color_;
}

void TiledSprite::Draw() const {
    if (!texture_) return;

    const Vector2& pos = Owner()->GlobalPosition();

    const float half_w = width_ * 0.5f;
    const float half_h = height_ * 0.5f;

    TileDrawContext ctx {  };
    ctx.sprite_pos = pos;
    ctx.s_left   = pos.x - half_w;
    ctx.s_top    = pos.y - half_h;
    ctx.s_right  = pos.x + half_w;
    ctx.s_bottom = pos.y + half_h;

    ctx.pixel = Engine::Instance().true_unit_size;
    ctx.u_to_tex_w = texture_width_ / unit_width_;
    ctx.u_to_tex_h = texture_height_ / unit_height_;
    ctx.rotation = Owner()->GlobalRotation();

    const float start_x = floor((ctx.s_left - offset_.x) / unit_width_) * unit_width_ + offset_.x;
    const float start_y = floor((ctx.s_top - offset_.y) / unit_height_) * unit_height_ + offset_.y;

    for (float y = start_y; y < ctx.s_bottom; y += unit_height_) {
        for (float x = start_x; x < ctx.s_right; x += unit_width_) {
            DrawSingleTile({x, y}, ctx);
        }
    }
}

void TiledSprite::DrawSingleTile(const Vector2& location, const TileDrawContext& ctx) const {

    const float ix0 = std::max(location.x, ctx.s_left);
    const float iy0 = std::max(location.y, ctx.s_top);
    const float ix1 = std::min(location.x + unit_width_,  ctx.s_right);
    const float iy1 = std::min(location.y + unit_height_, ctx.s_bottom);

    if (ix1 <= ix0 || iy1 <= iy0)
        return;

    const Rectangle source_rect = {
        (ix0 - location.x) * ctx.u_to_tex_w,
        (iy0 - location.y) * ctx.u_to_tex_h,
        (ix1 - ix0) * ctx.u_to_tex_w,
        (iy1 - iy0) * ctx.u_to_tex_h
    };

    const Rectangle dest_rect = {
        ctx.sprite_pos.x * ctx.pixel,
        ctx.sprite_pos.y * ctx.pixel,
        (ix1 - ix0) * ctx.pixel,
        (iy1 - iy0) * ctx.pixel
    };

    const Vector2 origin = {
        (ctx.sprite_pos.x - ix0) * ctx.pixel,
        (ctx.sprite_pos.y - iy0) * ctx.pixel
    };

    DrawTexturePro(*texture_, source_rect, dest_rect, origin, ctx.rotation, color_);
}
