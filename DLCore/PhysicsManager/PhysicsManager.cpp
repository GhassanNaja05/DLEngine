#include "PhysicsManager.h"

#include <valarray>

#include "../GastonMath.h"

#include "../../Components/PhysicsBody/PhysicsBody.h"
#include "../Debug/Debug.h"

#include "../Engine/Engine.h"

void PhysicsManager::Step(const float delta) const {

    /// Unground all sprites
    for (const auto body : bodies_) {
        body->is_grounded_ = false;
        body->snap_vector_ = {0.0f, 0.0f};
    }

    /// Update the shape to match their owners' global transform
    for (const auto body : bodies_) {
        body->UpdateShape();
    }

    /// Add gravity to each object
    for (const auto body : bodies_) {
        if (body->InverseMass() <= 0)
            continue;
        body->AddVelocity(
            Vector2{(body->IsXFrozen() ? 0.0f : gravity_.x), (body->IsYFrozen() ? 0.0f : gravity_.y)} *
            body->GravityScale() * delta);
    }
    /// Check the collisions
    for (int i = 0; i < Engine::PHYSICS_PASSES; ++i) {
        CheckCollisions();
        for (const auto body : bodies_) {
            body->ApplyPositionalCorrection();
        }
    }
}

void PhysicsManager::RegisterBody(PhysicsBody *body) {
    for (const auto b : bodies_) {
        if (b == body) {
            return;
        }
    }
    bodies_.push_back(body);
}

void PhysicsManager::UnregisterBody(PhysicsBody *body) {
    std::erase_if(bodies_, [body] (const auto& b) {
        if (b == body) {
            b->ResetVelocity();
            return true;
        }
        return false;
    });
}

Vector2& PhysicsManager::Gravity() {
    return gravity_;
}

void PhysicsManager::CheckCollisions() const {

    const auto body_count = bodies_.size();
    for (int i = 0; i < body_count; ++i) {
        const auto body_a = bodies_[i];
        if (!body_a->HasShapeComponent()) {
            continue;
        }
        ShapeComponent &shape_a = body_a->GetShapeComponent();

        for (int j = i + 1; j < body_count; ++j) {
            const auto body_b = bodies_[j];

            ShapeComponent &shape_b = body_b->GetShapeComponent();


            if (!CheckTouchingAABB(shape_a.GetShape().aabb_, shape_b.GetShape().aabb_)) {
                continue;
            }


            if (!body_b->HasShapeComponent()) {
                continue;
            }

            const auto l_a = body_a->GetLayer();
            const auto l_b = body_b->GetLayer();
            const auto e_a = body_a->GetExcludeLayers();
            const auto e_b = body_b->GetExcludeLayers();

            if ((l_a && (l_a & e_b) != 0) || (l_b && (l_b & e_a) != 0)) {
                continue;
            }

            if ((body_a->InverseMass() <= 0 || body_a->IsStatic()) &&
                (body_b->InverseMass() <= 0 || body_b->IsStatic())) {
                continue;
            }

            const Manifold &manifold = collision_table_[shape_a.GetShapeType()][shape_b.GetShapeType()](
                    shape_a.GetShape(), shape_b.GetShape());

            if (manifold) {

                const Vector2& n = manifold.normal_;

                const Vector2 relative_velocity = body_b->Velocity() - body_a->Velocity();
                const float velocity_along_normal = dot_product(relative_velocity, n);

                {
                    if (std::abs(n.x) > std::abs(n.y)) {

                        const float snap_val = (n.x > 0) ? -1.0f : 1.0f;
                        body_a->snap_vector_.x = snap_val;
                        body_b->snap_vector_.x = -snap_val;
                    } else {

                        const float snap_val = (n.y > 0) ? 1.0f : -1.0f;
                        body_a->snap_vector_.y = snap_val;
                        body_b->snap_vector_.y = -snap_val;

                        if (snap_val == -1.0f) body_a->is_grounded_ = true;
                        if (snap_val == 1.0f)  body_b->is_grounded_ = true;
                    }
                }

                float speed_a = std::sqrt(length_squared(body_a->Velocity()));
                float speed_b = std::sqrt(length_squared(body_b->Velocity()));

                ResolveCollision(manifold, body_b, body_a);

                const float friction_mult = body_a->Friction() * body_b->Friction();

                constexpr float threshold_mult = 10.0f / Engine::FPS;
                const float threshold =
                        threshold_mult * std::max(speed_a, speed_b) + 36.0f * threshold_mult + friction_mult;

                if (std::abs(velocity_along_normal) > threshold && velocity_along_normal > 0) {
                    body_a->OnCollision(body_b);
                    body_b->OnCollision(body_a);
                }
            }
        }
    }
}

bool PhysicsManager::CheckTouchingAABB(const AABB &a, const AABB &b) {
    return
        std::abs(a.position_.x - b.position_.x) <= a.half_width_ + b.half_width_ &&
        std::abs(a.position_.y - b.position_.y) <= a.half_height_ + b.half_height_;
}

void PhysicsManager::ResolveCollision(const Manifold& manifold, PhysicsBody* a, PhysicsBody* b) {

    const auto& normal = manifold.normal_;

    const float rv_x = b->Velocity().x - a->Velocity().x;
    const float rv_y = b->Velocity().y - a->Velocity().y;

    constexpr float percent = 0.2f;
    constexpr float k_slop = 0.01f;

    // Calculate relative velocity in terms of the normal direction
    const float vel_along_normal = dot_product(Vector2{rv_x, rv_y}, normal);

    // Do not resolve if velocities are separating
    if (vel_along_normal > 0)
        return;

    const bool static_a_x = a->IsXFrozen();
    const bool static_a_y = a->IsYFrozen();
    const bool static_b_x = b->IsXFrozen();
    const bool static_b_y = b->IsYFrozen();

    // Calculate restitution
    const float rest_a = a->Restitution();
    const float rest_b = b->Restitution();

    float e_x, e_y;

    if (static_a_x) {
        e_x = rest_b;
    } else if (static_b_x) {
        e_x = rest_a;
    } else {
        e_x = std::min(rest_a, rest_b);
    }

    if (static_a_y) {
        e_y = rest_b;
    } else if (static_b_y) {
        e_y = rest_a;
    } else {
        e_y = std::min(rest_a, rest_b);
    }

    const float im_a_x = static_a_x ? 0.0f : a->InverseMass();
    const float im_a_y = static_a_y ? 0.0f : a->InverseMass();
    const float im_b_x = static_b_x ? 0.0f : b->InverseMass();
    const float im_b_y = static_b_y ? 0.0f : b->InverseMass();

    const float total_inverse_mass =
        normal.x * normal.x * (im_a_x + im_b_x) +
        normal.y * normal.y * (im_a_y + im_b_y);

    if (total_inverse_mass > 0.0f) {
        const float j = -(1 + e_x) * vel_along_normal / total_inverse_mass;

        const float impulse_x = j * normal.x;

        const float vel_a_x = -impulse_x * im_a_x;
        const float vel_b_x =  impulse_x * im_b_x;

        a->AddVelocityX(vel_a_x);
        b->AddVelocityX(vel_b_x);

        const float correction_x = std::max(manifold.penetration_ - k_slop, 0.0f) / (im_a_x + im_b_x) * percent * manifold.normal_.x;
        if (!static_a_x && im_a_x > .0f) a->AddPositionalCorrection({-correction_x, 0.0f});
        if (!static_b_x && im_b_x > .0f) b->AddPositionalCorrection({ correction_x, 0.0f});
    }

    if (total_inverse_mass > 0.0f) {
        const float j = -(1 + e_y) * vel_along_normal / total_inverse_mass;

        const float impulse_y = j * normal.y;

        const float vel_a_y = -impulse_y * im_a_y;
        const float vel_b_y =  impulse_y * im_b_y;

        a->AddVelocityY(vel_a_y);
        b->AddVelocityY(vel_b_y);

        const float correction_y = std::max(manifold.penetration_ - k_slop, 0.0f) / (im_a_y + im_b_y) * percent * manifold.normal_.y;
        if (!static_a_y && im_a_y > .0f) a->AddPositionalCorrection({0.0f, -correction_y});
        if (!static_b_y && im_b_y > .0f) b->AddPositionalCorrection({0.0f,  correction_y});
    }

    ApplyFriction(a, b, -(1 + std::min(e_x, e_y)) * vel_along_normal / total_inverse_mass, normal);


    if constexpr (auto& [ax, ay] = a->Velocity(); true) {
        if (std::abs(ax) < 0.01f) a->SetVelocityX(0.0f);
        if (std::abs(ay) < 0.01f) a->SetVelocityY(0.0f);
    }

    if constexpr (auto& [bx, by] = b->Velocity(); true) {
        if (std::abs(bx) < 0.01f) b->SetVelocityX(0.0f);
        if (std::abs(by) < 0.01f) b->SetVelocityY(0.0f);
    }
}

void PhysicsManager::ApplyFriction(PhysicsBody* a, PhysicsBody* b, const float j, const Vector2& normal) {
    const Vector2 new_rv = b->Velocity() - a->Velocity();

    Vector2 tangent = new_rv - dot_product(new_rv, normal) * normal;
    if (const float l = std::sqrt(length_squared(tangent)); l <= 0.0f)
        return;
    else
        tangent = tangent / l;

    const float im_a_x = a->IsXFrozen() ? 0.0f : a->InverseMass();
    const float im_a_y = a->IsYFrozen() ? 0.0f : a->InverseMass();
    const float im_b_x = b->IsXFrozen() ? 0.0f : b->InverseMass();
    const float im_b_y = b->IsYFrozen() ? 0.0f : b->InverseMass();

    // Tangent effective mass (same logic as normal)
    const float tangent_mass =
        tangent.x * tangent.x * (im_a_x + im_b_x) +
        tangent.y * tangent.y * (im_a_y + im_b_y);

    if (tangent_mass == 0.0f)
        return;

    // Solve for magnitude to apply along the friction vector
    float jt = -dot_product(new_rv, tangent);
    jt /= tangent_mass;

    // Approximate combined friction coefficient
    const float mu_static  = (a->Friction() + b->Friction()) / 2.0f;
    const float mu_dynamic = mu_static * 0.8f;

    float friction_scalar;

    if (std::abs(jt) < j * mu_static)
        friction_scalar = jt;
    else
        friction_scalar = -j * mu_dynamic;

    Vector2 impulse = friction_scalar * tangent;

    a->AddVelocityX(-impulse.x * im_a_x);
    a->AddVelocityY(-impulse.y * im_a_y);

    b->AddVelocityX( impulse.x * im_b_x);
    b->AddVelocityY( impulse.y * im_b_y);
}

Manifold PhysicsManager::AABB_vs_AABB(const Shape &a, const Shape &b) {

    Manifold m { true };

    // Vector from A to B
    const AABB& aabbA = a.aabb_;
    const AABB& aabbB = b.aabb_;

    Vector2 min_a {
        aabbA.position_.x - aabbA.half_width_,
        aabbA.position_.y - aabbA.half_height_
    };

    Vector2 max_a {
        aabbA.position_.x + aabbA.half_width_,
        aabbA.position_.y + aabbA.half_height_
    };

    Vector2 min_b {
        aabbB.position_.x - aabbB.half_width_,
        aabbB.position_.y - aabbB.half_height_
    };

    Vector2 max_b {
        aabbB.position_.x + aabbB.half_width_,
        aabbB.position_.y + aabbB.half_height_
    };

    const auto a_position = (max_a + min_a) / 2.0f;
    const auto b_position = (max_b + min_b) / 2.0f;

    auto [x, y] = a_position - b_position;
    // Calculate half extents along x-axis for each object
    const float a_extent_x = (max_a.x - min_a.x) / 2.0f;
    const float b_extent_x = (max_b.x - min_b.x) / 2.0f;

    // Calculate overlap on x axis

    // SAT test on x-axis
    const float x_overlap = a_extent_x + b_extent_x - abs(x);
    if (x_overlap > 0)
    {
        // Calculate half extents along x-axis for each object
        const float a_extent_y = (max_a.y - min_a.y) / 2.0f;
        const float b_extent_y = (max_b.y - min_b.y) / 2.0f;

        // Calculate overlap on y axis

        // SAT test on y axis
        const float y_overlap = a_extent_y + b_extent_y - abs(y);
        if (y_overlap > 0)
        {
            // Find out which axis is axis of least penetration
            if(x_overlap < y_overlap)
            {
                // Point towards B knowing that n points from A to B
                if(x < 0)
                    m.normal_ = Vector2( -1, 0 );
                else
                    m.normal_ = Vector2( 1, 0 );
                m.penetration_ = x_overlap;
                return m;
            }
            // Point toward B knowing that n points from A to B
            if(y < 0)
                m.normal_ = Vector2( 0, -1 );
            else
                m.normal_ = Vector2( 0, 1 );
            m.penetration_ = y_overlap;
            return m;
        }
    }

    m.is_colliding_ = false;
    return m;
}

Manifold PhysicsManager::Circle_vs_AABB(const Shape &a, const Shape &b) {
    Manifold m = AABB_vs_Circle(b, a);
    m.penetration_ *= -1.0f;
    m.normal_ *= -1.0f;
    return m;
}

Manifold PhysicsManager::AABB_vs_Circle(const Shape& a, const Shape& b) {

    Manifold m { true };

    auto [aabb_half_width, aabb_half_height, aabb_position] = a.aabb_;
    auto [radius, position] = b.circle_;

    const Vector2 min_aabb = aabb_position - Vector2{aabb_half_width, aabb_half_height};
    const Vector2 max_aabb = aabb_position + Vector2{aabb_half_width, aabb_half_height};

    const Vector2 a_pos = (min_aabb + max_aabb) / 2.0f;
    const Vector2 b_pos = position;

    // Vector from A to B
    Vector2 n = a_pos - b_pos;

    // Closest point on A to center of B
    Vector2 closest = n;

    // Calculate half extents along each axis
    const float x_extent = (max_aabb.x - min_aabb.x) / 2.0f;
    const float y_extent = (max_aabb.y - min_aabb.y) / 2.0f;

    // Clamp point to edges of the AABB
    closest.x = std::clamp(closest.x, -x_extent, x_extent);
    closest.y = std::clamp(closest.y, -y_extent, y_extent);

    bool inside = false;

    // Circle is inside the AABB, so we need to clamp the circle's center
    // to the closest edge
    if (n == closest) {
        inside = true;

        // Find closest axis
        if (abs(n.x) > abs(n.y)) {
            // Clamp to closest extent
            if (closest.x > 0)
                closest.x = x_extent;
            else
                closest.x = -x_extent;
        }

        // y-axis is shorter
        else {
            // Clamp to closest extent
            if (closest.y > 0)
                closest.y = y_extent;
            else
                closest.y = -y_extent;
        }
    }

    const Vector2 normal = n - closest;
    float d = length_squared(normal);
    const float r = radius;

    // Early out of the radius is shorter than distance to the closest point and
    // Circle not inside the AABB
    if(d > r * r && !inside) {
        m.is_colliding_ = false;
        return m;
    }

        // Avoided sqrt until we needed
        d = sqrt( d );

        // Collision normal needs to be flipped to point outside if circle was
        // inside the AABB
        if(inside)
        {
            m.normal_ = -normal / d;
            m.penetration_ = r - d;
        }
        else
        {
            m.normal_ = normal / d;
            m.penetration_ = r - d;
        }

    return m;
}

Manifold PhysicsManager::Circle_vs_Circle(const Shape &a, const Shape &b) {
    Manifold m = {  };

    const auto &[radius_a_, position_a_] = a.circle_;
    const auto &[radius_b_, position_b_] = b.circle_;

    // Vector from A to B
    const Vector2 n = position_a_ - position_b_;

    // Circles have collided, now compute manifold
    const float d = std::sqrt(length_squared(n)); // perform actual sqrt

    const float r = radius_a_ + radius_b_;

    if (d > r)
        return Manifold { false };

    // If distance between circles is not zero
    if(d != 0) {
        // Distance is difference between radius and distance
        m.penetration_ = r - d;

        // Utilize our d since we performed sqrt on it already within Length( )
        // Points from A to B, and is a unit vector
        m.normal_ = n / d;
        m.is_colliding_ = true;
    }
    else {
        m.penetration_ = radius_a_;
        m.normal_ = Vector2(1,0);
        m.is_colliding_ = true;
    }

    return m;
}
