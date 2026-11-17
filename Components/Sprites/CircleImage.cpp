#include "CircleImage.h"
#include "../../DLCore/Engine/Engine.h"
#include "../../GameObject/GameObject.h"
#include "../../DLCore/GastonMath.h"

CircleImage::CircleImage() : Drawable(), unit_size_(Engine::Instance().true_unit_size) {  }

float &CircleImage::Radius() {
    return radius_;
}


Color& CircleImage::CircleColor() {
    return color_;
}

void CircleImage::Draw() const {
    DrawCircleV(Owner()->GlobalPosition() * unit_size_, radius_ * unit_size_, color_);
}
