#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H
#include "../../DLCore/GastonData.h"
#include "../Component.h"


class ShapeComponent : public Component {
public:
    ~ShapeComponent() override = default;

    /// Get the union containing the correct shape
    [[nodiscard]] virtual Shape GetShape() const = 0;

    /// Get the type of the shape
    [[nodiscard]] virtual ShapeType GetShapeType() const = 0;

    /// Update the shape data based on the owner's global transform
    virtual void UpdateShape() = 0;

    /// Calculate the shape's area
    [[nodiscard]] virtual float Area() const = 0;

protected:
    ShapeComponent() = default;

    Shape shape_ {  };
};



#endif
