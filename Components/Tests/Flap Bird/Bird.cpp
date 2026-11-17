#include "Bird.h"

#include "../../../DLCore/EventManager/CoreEvents.h"
#include "../../../DLCore/Modules/Input/Input.h"

void Bird::Setup() {
    auto& owner = *Owner();
    owner.AddTag(TagNames::Player);

    auto& image = owner.AddComponent<RectangleImage>();
    image.Width() = 0.8f;
    image.Height() = 0.8f;
    image.RectColor() = RED;

    auto& shape = owner.AddComponent<RectangleShape>();
    shape.SetupFromRectangleImage();

    auto& pb = owner.AddComponent<PhysicsBody>();
    pb.RegisterWithRectangleShape();
    pb.Restitution() = 0.2f;
    pb.GravityScale() = 1.5f;
    pb.AddVelocityX(4.0f);
    pb.Friction() = .0f;

    Listen<CollisionEvent>([&image](auto&, auto&) {
        image.RectColor() = Color(GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255);
    });
}

void Bird::Update(const float delta) {
    auto* pb = Owner()->GetComponent<PhysicsBody>();
    if (Input::IsKeyJustPressed(Input::Keys::SPACE)) {
        pb->ApplyForce(Vector2{0.0f, -8.0f});
    }
    if (const float v = pb->Velocity().y; v < -15.0f) {
        pb->AddVelocityY(-v - 15.0f);
    }


    GetCamera().target.x = Owner()->GlobalPosition().x;
}
