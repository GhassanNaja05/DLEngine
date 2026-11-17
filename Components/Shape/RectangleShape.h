#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H
#include <raylib.h>


#include "../../DLCore/GastonData.h"
#include "ShapeComponent.h"


class RectangleShape final : public ShapeComponent {

public:
    RectangleShape() = default;
    ~RectangleShape() override = default;

    /// Access the rectangle's width
    float& Width();

    /// Access the rectangle's height
    float& Height();

    /// Setup this component's width and height based on the owner's RectangleImage component, assumes its existence
    void SetupFromRectangleImage();
    void SetupFromSprite();

    void UpdateShape() override;
    [[nodiscard]] float Area() const override;

    [[nodiscard]] Shape GetShape() const override;
    [[nodiscard]] ShapeType GetShapeType() const override;

private:
    float width_ = 0;
    float height_ = 0;
};



#endif
