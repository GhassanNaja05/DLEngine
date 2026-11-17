#include "EntityManager.h"
#include <vector>

#include "../GastonData.h"

EntityManager & EntityManager::Instance() {
    static EntityManager instance;
    return instance;
}

GameObject& EntityManager::CreateEmptyChild(GameObject* parent) {
    auto child = GameObject::Create();
    parent->AddChild(std::move(child));
    return *parent->children_.back();
}

void EntityManager::CleanupDestroyed() const { root_->CleanupDestroyedChildren(); }

void EntityManager::DestroyAllChildren() const {

    root_->Destroy();
    root_->CleanupDestroyedChildren();
}

GameObject & GameObject::CreateChildInternal(
    GameObject& parent,
    void (*add_components)(GameObject&)
) {
    auto child = Create();
    auto& ref = *child;

    add_components(ref);
    parent.AddChild(std::move(child));

    return *parent.add_queue_.back();
}

GameObject& GameObject::CreateEmptyChildInternal(GameObject& parent) {
    parent.AddChild(Create());
    return *parent.children_.back();
}
