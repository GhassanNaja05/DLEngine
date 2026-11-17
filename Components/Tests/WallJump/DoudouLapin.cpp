#include "DoudouLapin.h"

#include "../../../DLCore/GastonMath.h"
#include "../../../DLCore/Modules/Input/Input.h"
#include "../../PhysicsBody/PhysicsBody.h"
#include "../../PhysicsBody/PhysicsLayers.h"
#include "../../Shape/RectangleShape.h"
#include "../../Sprites/RectangleImage.h"


void DoudouLapin::Setup() {


    Owner()->SetPosition({0.0f, 0.0f});

    auto &sprite = Owner()->AddComponent<RectangleImage>();
    sprite.Width() = 1.2f;
    sprite.Height() = 2.0f;
    sprite.RectColor() = {245, 167, 45, 255};
    sprite.Layer() = 2;

    auto &shape = Owner()->AddComponent<RectangleShape>();
    shape.SetupFromRectangleImage();

    auto &pb = Owner()->AddComponent<PhysicsBody>();
    pb.RegisterWithRectangleShape();
    pb.GravityScale() = 1.0f;
    pb.Restitution() = .0f;
    pb.Friction() = .0f;

    p = &pb;

    pb.ExcludeAllLayers();
    pb.ToggleExcludeLayer(PhysicsLayers::Wall);
    pb.ToggleLayer(PhysicsLayers::Player);

}

void DoudouLapin::Update(float delta) {

    const bool is_on_ground = p->IsTouchingGround();
    const float snap_x = p->GetSnapVector().x;
    const float raw_x = Input::Axes::GetHorizontalRaw<0>();

    const Vector2 velocity = p->Velocity();
    const float input = raw_x * 5.0f;
    const bool jump_requested = Input::IsKeyJustPressed(Input::Keys::SPACE);

    const bool sense_input = raw_x != 0.0f;

    const float jump = -10.0f;

    const bool is_snapped_to_wall = snap_x != 0.0f;

    bool change_x = false;
    bool change_y = false;

    Vector2 new_velocity = {0.0f};


    switch (state) {
    case 0:

        if (!input_cooldown) {
            new_velocity.x = input;
            change_x = true;
        }

        if (jump_requested) {
            new_velocity.y = jump;
            change_y = true;
            state = 1;
            if (is_snapped_to_wall) {
                last_snap = snap_x;
                input_cooldown = true;
                CreateSingleShotTimer(0.1f, [this] { input_cooldown = false; });
                was_cornered = true;
            }
        }
        else if (!is_on_ground) {
            state = 2;
        }
        break;

    case 1:
        if (!input_cooldown) {
            new_velocity.x = input;
            change_x = true;
        }

        if (velocity.y >= 0.0f)
            state = 2;

        if (sense_input && raw_x == snap_x) was_cornered = false;

        if (is_snapped_to_wall && !input_cooldown && !was_cornered) {
            state = 3;
            new_velocity.y = 0.0f;
            change_y = true;
        }

        if (jump_requested && is_snapped_to_wall) {
            state = 4;
            new_velocity.y = jump;
            new_velocity.x = -snap_x * 5.0f;
            change_x = true;
            change_y = true;
            input_cooldown = true;
            CreateSingleShotTimer(0.2f, [this] { input_cooldown = false; });
        }

        break;

    case 2:
        if (!input_cooldown) {
            new_velocity.x = input;
            change_x = true;
        }

        if (is_on_ground)
            state = 0;

        if (is_snapped_to_wall) {
            last_snap = snap_x;
            state = 3;
            new_velocity.y = 0.0f;
            change_y = true;
        }
        break;

    case 3:
        if (sense_input && snap_x != raw_x) {
            state = 2;
        }
        if (jump_requested) {
            state = 4;
            new_velocity.y = jump;
            new_velocity.x = -snap_x * 5.0f;
            change_x = true;
            change_y = true;
            input_cooldown = true;
            CreateSingleShotTimer(0.2f, [this] { input_cooldown = false; });
        }
        else if (is_on_ground) {
            state = 0;
        }
        if (!is_snapped_to_wall) {
            state = 2;
        }
        break;

    case 4:
        if (sense_input && !input_cooldown) {
            state = 1;
            new_velocity.x = input;
            change_x = true;
        }

        if (is_on_ground)
            state = 0;
        else if (is_snapped_to_wall && last_snap != snap_x) {
            new_velocity.y = 0.0f;
            change_y = true;
            last_snap = snap_x;
            state = 3;
        }

        break;
    default:;
    }

    if (change_x) p->SetVelocityX(new_velocity.x);
    if (change_y) p->SetVelocityY(new_velocity.y);

    GetCamera().target = Owner()->Position();
}
