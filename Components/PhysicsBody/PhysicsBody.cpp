#include "PhysicsBody.h"

#include "../../DLCore/Debug/Debug.h"
#include "../../DLCore/EventManager/CoreEvents.h"
#include "../../DLCore/EventManager/EventManager.h"
#include "../../DLCore/GastonMath.h"
#include "../../DLCore/PhysicsManager/PhysicsManager.h"
#include "../Shape/CircleShape.h"
#include "../Shape/RectangleShape.h"


void PhysicsBody::Update(float delta) {

}

void PhysicsBody::PhysicsUpdate(const float delta) {
    if (!is_static_) {
        Owner()->Translate(velocity_ * delta);
        velocity_ += forces_ * inverse_mass_;
    }
    forces_ = {  };
}

void PhysicsBody::Destroy() {
    PhysicsManager::Instance().UnregisterBody(this);
}


bool PhysicsBody::HasShapeComponent() const {
    return shape_component_ != nullptr;
}

ShapeType PhysicsBody::GetShapeType() const {
    return shape_component_->GetShapeType();
}

ShapeComponent& PhysicsBody::GetShapeComponent() const {
    if (shape_component_ == nullptr) {
        throw std::invalid_argument("PhysicsBody::GetShapeComponent: shape component is null");
    }
    return *shape_component_;
}

void PhysicsBody::RegisterWithRectangleShape() {
    RegisterSelf(Owner()->GetComponent<RectangleShape>());
    assert(shape_component_ != nullptr);
}

void PhysicsBody::RegisterWithCircleShape() {
    RegisterSelf(Owner()->GetComponent<CircleShape>());
    assert(shape_component_ != nullptr);
}

void PhysicsBody::RegisterSelf(ShapeComponent *shape_component) {
    shape_component_ = shape_component;
    PhysicsManager::Instance().RegisterBody(this);
}

bool PhysicsBody::IsTouchingGround() const {
    return is_grounded_;
}

const Vector2& PhysicsBody::GetSnapVector() const {
    return snap_vector_;
}

bool PhysicsBody::IsStatic() const {
    return is_static_ || (freeze_x_ && freeze_y_);
}

void PhysicsBody::SetStatic(const bool is_static) {
    is_static_ = is_static;
    if (is_static_) {
        SetInfiniteMass();
        velocity_ = {0.0f, 0.0f};
    } else {
        SetDensity(density_ > 0.0f ? density_ : 1.0f);
    }
}

void PhysicsBody::FreezeX(const bool f) {
    freeze_x_ = f;
    if (freeze_x_) {
        if (freeze_y_) {
            SetStatic(true);
        }
        else
            SetVelocityX(0.0f);
    }
}

void PhysicsBody::FreezeY(const bool f) {
    freeze_y_ = f;
    if (freeze_y_) {
        if (freeze_x_) {
            SetStatic(true);
        }
        else
            SetVelocityY(0.0f);
    }
}

bool PhysicsBody::IsXFrozen() const {
    return freeze_x_ || is_static_;
}

bool PhysicsBody::IsYFrozen() const {
    return freeze_y_ || is_static_;
}


void PhysicsBody::UpdateShape() const {
    if (shape_component_ == nullptr)
        return;

    shape_component_->UpdateShape();
}

void PhysicsBody::AddVelocity(const Vector2 &velocity) {
    if (is_static_)
        return;
    velocity_ += velocity;
}

void PhysicsBody::SetVelocity(const Vector2 &velocity) {
    if (is_static_)
        return;
    velocity_ = velocity;
}


void PhysicsBody::AddVelocityX(const float velocity) {
    if (is_static_ || freeze_x_)
        return;
    velocity_.x += velocity;
}

void PhysicsBody::SetVelocityX(const float velocity) {
    if (is_static_ || freeze_x_)
        return;
    velocity_.x = velocity;
}


void PhysicsBody::AddVelocityY(const float velocity) {
    if (is_static_ || freeze_y_)
        return;
    velocity_.y += velocity;
}

void PhysicsBody::SetVelocityY(const float velocity) {
    if (is_static_ || freeze_y_)
        return;
    velocity_.y = velocity;
}


void PhysicsBody::ApplyForce(const Vector2& force) {
    forces_ += force;
}


void PhysicsBody::OnCollision(const PhysicsBody *other) {
    if (!other)
        return;

    EventManager::Instance().Emit<CollisionEvent>(
            *this, {*Owner(), *other->Owner()},
            {[this](const Component &physics_body) { return physics_body.Owner() == this->Owner(); }});
}

void PhysicsBody::ToggleLayer(const unsigned int layer) {
    assert((layer - 1 & layer) == 0 && layer != 0);
    physics_layers_ ^= layer;
}

void PhysicsBody::ToggleExcludeLayer(const unsigned int layer) {
    assert((layer - 1 & layer) == 0 && layer != 0);
    exclude_layers_ ^= layer;
}

void PhysicsBody::ExcludeAllLayers() {
    exclude_layers_ = -1;
}

unsigned int PhysicsBody::GetLayer() const {
    return physics_layers_;
}

unsigned int PhysicsBody::GetExcludeLayers() const {
    return exclude_layers_;
}

void PhysicsBody::ResetVelocity() { velocity_ = {0.0f, 0.0f}; }

void PhysicsBody::AddPositionalCorrection(const Vector2 &amount) {
    if (is_static_)
        return;
    positional_correction_ += amount;
}

void PhysicsBody::ApplyPositionalCorrection() {
    if (!is_static_) {
        Owner()->Translate( {
            freeze_x_ ? 0.0f : positional_correction_.x,
            freeze_y_ ? 0.0f : positional_correction_.y,
        });
    }
    positional_correction_ = {0.0f, 0.0f};
}

const Vector2& PhysicsBody::Velocity() const {
    return velocity_;
}

float PhysicsBody::AngularVelocity() const {
    return angular_velocity_;
}

float PhysicsBody::InverseMass() const {
    if (is_static_) return 0.0f;
    return inverse_mass_;
}

void PhysicsBody::SetInfiniteMass() {
    inverse_mass_ = 0.0f;
}

void PhysicsBody::SetDensity(const float density) {
    density_ = density;
    inverse_mass_ = 1.0f / (density * shape_component_->Area());
}

float PhysicsBody::Density() const {
    return is_static_ || inverse_mass_ == 0.0f ? -.1f : density_;
}

float &PhysicsBody::Restitution() {
    return restitution_;
}

float &PhysicsBody::Friction() {
    return friction_;
}

float &PhysicsBody::GravityScale() {
    return gravity_scale_;
}
