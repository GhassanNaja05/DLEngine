#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
#include "../GastonData.h"


class PhysicsManager {
public:
    static PhysicsManager& Instance() {
        static PhysicsManager instance;
        return instance;
    }

    /// Called each frame, the procedure to do to check and resolve collisions
    void Step(float delta) const;

    /// Register a new body to be affected by the PhysicsManager
    void RegisterBody(PhysicsBody* body);

    /// Unregister a body
    void UnregisterBody(PhysicsBody* body);

    /// Access the gravity constant
    Vector2& Gravity();

private:

    PhysicsManager() = default;

    /// Check the collision between each registered body
    void CheckCollisions() const;

    /// Checks if the AABB of two objects are touching
    [[nodiscard]] static bool CheckTouchingAABB(const AABB & aabb1, const AABB & aabb2) ;

    static void ApplyFriction(PhysicsBody* a, PhysicsBody* b, float j, const Vector2& normal);

    /// Resolve the collisions between the object (calculate impulse, apply velocity change)
    static void ResolveCollision(const Manifold& manifold, PhysicsBody* a, PhysicsBody* b);

    std::vector<PhysicsBody*> bodies_ = std::vector<PhysicsBody*>();

    /// Comparison between AABB and AABB
    [[nodiscard]] static Manifold AABB_vs_AABB(const Shape &a, const Shape &b);

    /// Comparison between circle and AABB
    [[nodiscard]] static Manifold Circle_vs_AABB(const Shape &a, const Shape &b);

    /// Comparison between AABB and circle
    [[nodiscard]] static Manifold AABB_vs_Circle(const Shape &a, const Shape &b);

    /// Comparison between circle and circle
    [[nodiscard]] static Manifold Circle_vs_Circle(const Shape &a, const Shape &b);

    using CollisionFunction = Manifold (*)(const Shape&, const Shape&);

    /// Table to pick the correction comparison function
    const CollisionFunction collision_table_[2][2] = {
        { AABB_vs_AABB, AABB_vs_Circle },
        { Circle_vs_AABB, Circle_vs_Circle }
    };

    Vector2 gravity_ = { 0.0f, 9.81f };

};



#endif
