#ifndef SPRITE_H
#define SPRITE_H
#include <string>


#include "../Component.h"
#include "../Drawable/Drawable.h"
#include "raylib.h"


class Sprite final : public Drawable {

public:
    Sprite() = default;
    ~Sprite() override = default;

    [[nodiscard]] float Width() const;
    [[nodiscard]] float Height() const;

    void SetImage(const std::string& image);

    float& Scale();
    void ScaleToWidth(float width);
    void ScaleToHeight(float height);

    void Draw() const override;

    Color& DrawColor();

private:

    Color color_ = WHITE;

    std::shared_ptr<Texture2D> texture_ = nullptr;

    float internal_scale_ = 1.0f;
    float texture_width_ = 0.0f;
    float texture_height_ = 0.0f;
    float texture_half_width_ = .0f;
    float texture_half_height_ = .0f;


};



#endif
