#ifndef CIRCLEIMAGE_H
#define CIRCLEIMAGE_H
#include <raylib.h>


#include "../Component.h"
#include "../Drawable/Drawable.h"


class CircleImage final : public Drawable {
public:
    CircleImage();
    ~CircleImage() override = default;

    void Draw() const override;

    /// Access the radius of the circle
    float& Radius();

    /// Access the color of the circle
    Color& CircleColor();

private:
    float radius_ { 1.0f };
    Color color_ {  WHITE };

    const float unit_size_;
};



#endif
