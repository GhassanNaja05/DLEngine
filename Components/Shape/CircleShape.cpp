#include "CircleShape.h"
#include "../../DLCore/GastonMath.h"
#include "../Sprites/CircleImage.h"

float &CircleShape::Radius() {
    return radius_;
}

void CircleShape::SetupFromCircleImage() {
    auto* circle_image = Owner()->GetComponent<CircleImage>();

    radius_ = circle_image->Radius();
}

void CircleShape::UpdateShape() {
    shape_.circle_.position_ = Owner()->GlobalPosition();
    shape_.circle_.radius_ = Owner()->GlobalScale() * radius_;

    shape_.aabb_.position_ = shape_.circle_.position_;
    shape_.aabb_.half_width_ = shape_.circle_.radius_;
    shape_.aabb_.half_height_ = shape_.circle_.radius_;
}

float CircleShape::Area() const {
    return radius_ * radius_ * PI * Owner()->GlobalScale();
}

Shape CircleShape::GetShape() const {
    return shape_;
}

ShapeType CircleShape::GetShapeType() const {
    return CircleType;
}
