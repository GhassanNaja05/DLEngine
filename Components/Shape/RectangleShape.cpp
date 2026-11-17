#include "RectangleShape.h"
#include "../../DLCore/GastonMath.h"
#include "../Sprites/RectangleImage.h"
#include "../Sprites/Sprite.h"


float &RectangleShape::Width() {
    return width_;
}

float &RectangleShape::Height() {
    return height_;
}

void RectangleShape::SetupFromRectangleImage() {
    auto *rectangle_image = Owner()->GetComponent<RectangleImage>();

    width_ = rectangle_image->Width();
    height_ = rectangle_image->Height();
}

void RectangleShape::SetupFromSprite() {
    auto *rectangle_image = Owner()->GetComponent<Sprite>();

    width_ = rectangle_image->Width();
    height_ = rectangle_image->Height();
}

void RectangleShape::UpdateShape() {

    const float global_scale = Owner()->GlobalScale();
    const float width = global_scale * width_;
    const float height = global_scale * height_;
    const Vector2& global_position = Owner()->GlobalPosition();

    shape_.aabb_.half_width_ = width / 2.0f;
    shape_.aabb_.half_height_ = height / 2.0f;
    shape_.aabb_.position_ = global_position;
}

float RectangleShape::Area() const {
    return width_ * height_ * Owner()->GlobalScale();
}

Shape RectangleShape::GetShape() const {
    return shape_;
}

ShapeType RectangleShape::GetShapeType() const { return AABBType; }
