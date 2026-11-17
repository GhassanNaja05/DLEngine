#include "PillarSpawner.h"

#include "../../../DLCore/EntityManager/EntityManager.h"
#include "../../../DLCore/Engine/Engine.h"
#include "Pillar.h"

#include "../../../DLCore/Debug/Debug.h"


void PillarSpawner::Update(const float delta) {
    float global_x = EntityManager::Instance().Camera().target.x;
    accumulator += global_x - previous_x;

    while (accumulator > next) {
        (void) Owner()->CreateChild<Pillar>([this](auto& owner, auto* pillar) {
            owner.TranslateX(next_pillar + 15.0f);
            owner.TranslateY(GetRandomValue(-5, 5));
        });
        accumulator -= next;
        next_pillar += next;
    }

    previous_x = global_x;
}
