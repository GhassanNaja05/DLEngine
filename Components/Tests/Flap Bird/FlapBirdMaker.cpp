#include "FlapBirdMaker.h"

#include "../../../DLCore/Engine/Engine.h"
#include "../../ScriptComponent/LambdaScript.h"
#include "../../Shape/RectangleShape.h"
#include "Bird.h"
#include "PillarSpawner.h"

void FlapBirdMaker::Setup() {

    auto& root = EntityManager::Instance().GetRoot();

    auto& spawner = root.CreateChild<PillarSpawner>();
    auto& bird = root.CreateChild<Bird>();

    auto& wall_up = root.CreateChild<RectangleShape, PhysicsBody, LambdaScript>([](auto& owner, auto* rect_shape, auto* physics_body, auto* lambda) {
        const float screen_width = Engine::ScreenX();
        const float screen_units = screen_width / Engine::Instance().true_unit_size;

        rect_shape->Width() = screen_units;
        rect_shape->Height() = 1.0f;

        owner.TranslateY(-7.5f);

        physics_body->RegisterWithRectangleShape();
        physics_body->SetStatic(true);
        physics_body->Friction() = .0f;

        lambda->OverrideUpdate([](auto& script, float delta) {
            script.Owner()->SetPosition(Vector2{EntityManager::Instance().Camera().target.x, script.Owner()->Position().y});
        });
    });

    auto& wall_down = root.CreateChild<RectangleShape, PhysicsBody, LambdaScript>([](auto& owner, auto* rect_shape, auto* physics_body, auto* lambda) {
        const float screen_width = Engine::ScreenX();
        const float screen_units = screen_width / Engine::Instance().true_unit_size;

        rect_shape->Width() = screen_units;
        rect_shape->Height() = 1.0f;

        owner.TranslateY(7.5f);

        physics_body->RegisterWithRectangleShape();
        physics_body->SetStatic(true);
        physics_body->Friction() = .0f;

        lambda->OverrideUpdate([](auto& script, float delta) {
            script.Owner()->SetPosition(Vector2{EntityManager::Instance().Camera().target.x, script.Owner()->Position().y});
        });
    });
}
