#ifndef SQUAREIMAGE_H
#define SQUAREIMAGE_H
#include <raylib.h>


#include "../Component.h"
#include "../Drawable/Drawable.h"


class Engine;
class RectangleImage final : public Drawable {
public:
    RectangleImage();
    ~RectangleImage() override = default;

    /// Represent the rectangle on screen
    void Draw() const override;

    /// Access the width of the rectangle
    float& Width();
    /// Access the height of the rectangle
    float& Height();
    /// Access the color of the rectangle
    Color& RectColor();

private:
    float width_ { 2.0f };
    float height_ { 2.0f };
    Color color_ { WHITE };

    const float unit_size_;
};



#endif
