#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <raylib.h>


#include "../Component.h"


class UserInterface : public Component {

public:
    ~UserInterface() override = default;

    virtual void Render() const = 0;

    [[nodiscard]] float GetOffsetX() const;
    [[nodiscard]] float GetOffsetY() const;

    void SetOffsetX(float offset);
    void SetOffsetY(float offset);

    [[nodiscard]] float GetWidthX() const;
    [[nodiscard]] float GetWidthY() const;

    [[nodiscard]] float GetHeightX() const;
    [[nodiscard]] float GetHeightY() const;

    void SetWidthX(float width);
    void SetWidthY(float width);

    void SetHeightX(float height);
    void SetHeightY(float height);

protected:

    struct UIConfig {
        Vector2 center_ = Vector2(0,0);
        float half_width_ = 0;
        float half_height_ = 0;
        float rotation_ = 0;
    };

    enum Horizontal {
        Left = -1,
        Middle_H = 0,
        Right = 1,
    };

    enum Vertical {
        Up = -1,
        Middle_V = 0,
        Down = 1,
    };

    struct Alignment {
        Horizontal h = Left;
        Vertical v = Up;
    };

    UserInterface() = default;

    Alignment internal_anchor_;
    Alignment external_anchor_;

    UIConfig config_;
};



#endif
