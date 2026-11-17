#ifndef TILEDSPRITE_H
#define TILEDSPRITE_H
#include "../Drawable/Drawable.h"


class TiledSprite final : public Drawable {
public:
    TiledSprite() = default;
    ~TiledSprite() override = default;

    [[nodiscard]] float Width() const;
    [[nodiscard]] float Height() const;

    void SetImage(const std::string& image);

    float& Scale();
    void SetUnitWidth(float width);
    void SetUnitHeight(float height);
    [[nodiscard]] float GetUnitWidth() const;
    [[nodiscard]] float GetUnitHeight() const;

    void SetWidth(float width);
    void SetHeight(float height);
    [[nodiscard]] float GetWidth() const;
    [[nodiscard]] float GetHeight() const;

    Vector2 GetOffset() const;
    void SetOffset(const Vector2& offset);

    void Draw() const override;

    Color& DrawColor();

private:

    Color color_ = WHITE;

    struct TileDrawContext {
        float s_left, s_top, s_right, s_bottom;
        float u_to_tex_w, u_to_tex_h;
        float pixel;
        Vector2 sprite_pos;
        float rotation;
    };

    void DrawSingleTile(const Vector2& location, const TileDrawContext& ctx) const;

    std::shared_ptr<Texture2D> texture_ = nullptr;

    float internal_scale_ = 1.0f;

    float texture_width_ = 0.0f;
    float texture_height_ = 0.0f;
    float texture_half_width_ = .0f;
    float texture_half_height_ = .0f;

    float width_ = 0.0f;
    float height_ = 0.0f;

    float unit_width_ = 0.0f;
    float unit_height_ = 0.0f;

    Vector2 offset_ = Vector2(0, 0);
};



#endif
