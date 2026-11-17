#include "Pillar.h"

#include "../../../DLCore/Debug/Debug.h"
#include "../../../DLCore/Engine/Engine.h"
#include "../../../DLCore/EntityManager/EntityManager.h"
#include "../../../DLCore/EventManager/CoreEvents.h"
#include "../../EventListener/EventListener.h"
#include "../../PhysicsBody/PhysicsBody.h"
#include "../../Shape/RectangleShape.h"
#include "../../Sprites/RectangleImage.h"

void Pillar::Setup() {
    auto& owner = *Owner();

    auto& upper = owner.CreateChild<RectangleImage, RectangleShape, PhysicsBody, EventListener>([](auto& owner, auto* rect_img, auto* rect_shape, auto* pb, auto* listener) {
        rect_img->Width() = 1.0f;
        rect_img->Height() = 12.0f;
        rect_img->RectColor() = BLUE;

        rect_shape->SetupFromRectangleImage();

        pb->RegisterWithRectangleShape();
        pb->SetStatic(true);

        owner.TranslateY(-2.0f - 6.0f);

        listener->template Listen<CollisionEvent>([](const Component & caller, const CollisionEvent& event) {
            if (event.other_.HasTag(TagNames::Player)) {
                event.other_.Destroy();
            }
        });
    });

    auto& lower = owner.CreateChild<RectangleImage, RectangleShape, PhysicsBody, EventListener>([](auto& owner, auto* rect_img, auto* rect_shape, auto* pb, auto* listener) {
        rect_img->Width() = 1.0f;
        rect_img->Height() = 12.0f;
        rect_img->RectColor() = BLUE;

        rect_shape->SetupFromRectangleImage();

        pb->RegisterWithRectangleShape();
        pb->SetStatic(true);

        owner.TranslateY(2.0f + 6.0f);

        listener->template Listen<CollisionEvent>([](const Component & caller, const CollisionEvent& event) {
            if (event.other_.HasTag(TagNames::Player)) {
                event.other_.Destroy();
            }
        });
    });
}

void Pillar::Update(float delta) {
    if (Owner()->GlobalPosition().x < EntityManager::Instance().Camera().target.x - 20.0f) {
        Owner()->Destroy();
    }
}
