#ifndef GASTONDATA_H
#define GASTONDATA_H
#include <functional>
#include <raylib.h>

#include "../GameObject/GameObject.h"

class Component;
class PhysicsBody;

/// A representation of a GameObject's coordinates, having both local and global coordinates
struct Transform2D {
    /// Local position relative to parent
    Vector2 position { 0.0f, 0.0f };
    /// Local rotation relative to parent, in degrees
    float rotation { 0.0f };
    /// Local scale relative to parent
    float scale { 1.0f };

    /// Global position relative to world origin
    Vector2 global_position { 0.0f, 0.0f };
    /// Global rotation relative to world origin, in degrees
    float global_rotation { 0.0f };
    /// Global scale relative to world origin
    float global_scale { 1.0f };

    /// Indicates whether global position needs to be recalculated or not
    bool dirty { true };
};

/// Describes the state of the current camera : position, tilt, rotation
struct CameraConfig {
    /// Position of the camera relative to the world origin
    Vector2 target { 0.0f, 0.0f };

    /// Tilt level of the camera
    float tilt { 0.0f };

    /// Zoom level of the camera
    float zoom { 1.0f };
};

/// Representation of a Circle, used for collision detection
struct Circle {
    float radius_ { 0.0f };
    Vector2 position_ { 0.0f, 0.0f };
};

/// Representation of a AABB, used for collision detection
struct AABB {
    /// Half width
    float half_width_ { 0.0f };

    /// Half height
    float half_height_ { 0.0f };

    /// Global position
    Vector2 position_ { 0.0f, 0.0f };
};

/// Union for AABB and Circle shapes
struct Shape {
    AABB aabb_;
    Circle circle_;
};

/// Data type representing a collision, with the penetration and normal vector
struct Manifold {
    Manifold() = default;
    explicit Manifold(const bool is_colliding, const float penetration,
             const Vector2 &normal) :
        is_colliding_(is_colliding), penetration_(penetration), normal_(normal) {}
    explicit Manifold(const bool is_colliding) : is_colliding_ (is_colliding) {}

    bool is_colliding_ { false };
    float penetration_ { 0.0f };
    Vector2 normal_ { 0.0f, 0.0f };

    explicit operator bool() const { return is_colliding_; }
};

/// Enum to distinguish shape types
enum ShapeType {
    AABBType,
    CircleType,
};

/// The different tags to distinguish game objects
namespace TagNames {
    constexpr uint32_t Player = 1;
    constexpr uint32_t Enemy = 2;
    constexpr uint32_t Other = 4;
    constexpr uint32_t Ball = 8;
}

#endif
