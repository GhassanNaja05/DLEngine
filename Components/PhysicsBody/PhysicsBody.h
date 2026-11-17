#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H
#include <raylib.h>


#include "../../DLCore/GastonData.h"
#include "../Component.h"
#include "../Shape/ShapeComponent.h"


class ShapeComponent;
class PhysicsBody final : public Component {

public:
    PhysicsBody() = default;
    ~PhysicsBody() override = default;

    void Update(float delta) override;
    void PhysicsUpdate(float delta) override;

    /// Register the RectangleShape owned by the owner, assumes its existence
    void RegisterWithRectangleShape();

    /// Register the CircleShape owned by the owner, assumes its existence
    void RegisterWithCircleShape();

    /// Checks if this component has a registered Shape component
    [[nodiscard]] bool HasShapeComponent() const;

    /// Return the type of the shape registered
    [[nodiscard]] ShapeType GetShapeType() const;

    /// Return the registered shape
    [[nodiscard]] ShapeComponent& GetShapeComponent() const;

    /// Checks if this PhysicsBody is static
    [[nodiscard]] bool IsStatic() const;

    /// Make this PhysicsBody static or not
    void SetStatic(bool is_static);

    /// Freeze this PhysicsBody on the x-axis
    void FreezeX(bool f);

    /// Freeze this PhysicsBody on the y-axis
    void FreezeY(bool f);

    bool IsXFrozen() const;

    bool IsYFrozen() const;

    /// Add a velocity vector
    void AddVelocity(const Vector2& velocity);

    /// Set the velocity to the given value
    void SetVelocity(const Vector2& velocity);

    /// Add velocity to the x-direction
    void AddVelocityX(float velocity);

    /// Set the x velocity
    void SetVelocityX(float velocity);

    /// Add velocity to the y-direction
    void AddVelocityY(float velocity);

    /// Set the y velocity
    void SetVelocityY(float velocity);

    /// Apply force at the end of the frame
    void ApplyForce(const Vector2& force);

    /// Return a vector representing the velocity
    [[nodiscard]] const Vector2& Velocity() const;

    /// Return a vector representing the angular velocity
    [[nodiscard]] float AngularVelocity() const;

    /// Return a value representing the inverse mass
    [[nodiscard]] float InverseMass() const;

    /// Set the inverse mass
    void SetInfiniteMass();

    /// Set the density, used to calculate the mass along with the area
    void SetDensity(float density);

    /// Return a value representing the density
    [[nodiscard]] float Density() const;

    /// Access the restitution
    float& Restitution();

    /// Access the friction
    float& Friction();

    /// Access the gravity scale
    float& GravityScale();

    /// Toggle this PhysicsBody for the given layer
    void ToggleLayer(unsigned int layer);

    /// Toggle this PhysicsBody to exclude the given layer
    void ToggleExcludeLayer(unsigned int layer);

    /// Enable all exclude layers
    void ExcludeAllLayers();

    /// Check if this PhysicsBody is touching the ground
    [[nodiscard]] bool IsTouchingGround() const;

    /// Return the snapping vector for this physics body
    [[nodiscard]] const Vector2 &GetSnapVector() const;

    [[nodiscard]] unsigned int GetLayer() const;
    [[nodiscard]] unsigned int GetExcludeLayers() const;

private:
    void Destroy() override;

    /// Trigger a collision event
    void OnCollision(const PhysicsBody * other);

    /// Set the velocity to 0
    void ResetVelocity();

    /// Update registered shape
    void UpdateShape() const;

    /// Add positional correction to apply to the object
    void AddPositionalCorrection(const Vector2& amount);

    /// Apply the added positional correction
    void ApplyPositionalCorrection();

    /// Register this component to react to physics
    void RegisterSelf(ShapeComponent* shape_component);

    unsigned int physics_layers_ = 0;
    unsigned int exclude_layers_ = 0;

    ShapeComponent* shape_component_ = nullptr;

    bool is_static_ { false };
    bool freeze_x_ { false };
    bool freeze_y_ { false };

    float inverse_mass_ { 1.0f };

    Vector2 velocity_ {  };
    float angular_velocity_ {  };

    Vector2 forces_ {  };

    Vector2 positional_correction_ {  };

    float density_ { 1.0f };
    float restitution_ { .30f };

    float friction_ { .15f };
    float gravity_scale_ { 1.0f };

    bool is_grounded_ { false };
    Vector2 snap_vector_ { 0.0f, 0.0f };

    friend class PhysicsManager;
};



#endif
