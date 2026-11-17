#ifndef CANVAS_H
#define CANVAS_H
#include <raylib.h>


#include "../Component.h"


class Canvas : public Component {
public:
    ~Canvas() override = default;



protected:
    Canvas() = default;

    //virtual void ConfigureRect(const UIConfig& config, UIRect& rect) const;

    [[nodiscard]] virtual float GetHalfWidth() const = 0;
    [[nodiscard]] virtual float GetHalfHeight() const = 0;

    //[[nodiscard]] virtual Vector2 Canvas::GetAnchor(Horizontal h, Vertical v) const = 0;

    //UIConfig config;
};



#endif
