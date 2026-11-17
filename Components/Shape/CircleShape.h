#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H
#include <raylib.h>


#include "../../DLCore/GastonData.h"
#include "ShapeComponent.h"


class CircleShape final : public ShapeComponent {

public:
    CircleShape() = default;
    ~CircleShape() override = default;

    void UpdateShape() override;
    [[nodiscard]] float Area() const override;

    /// Access the radius
    float& Radius();

    /// Setup this component's radius based on the owner's CircleImage component, assumes its existence
    void SetupFromCircleImage();

    [[nodiscard]] Shape GetShape() const override;
    [[nodiscard]] ShapeType GetShapeType() const override;

private:
    float radius_ = 1.0f;

};



#endif
