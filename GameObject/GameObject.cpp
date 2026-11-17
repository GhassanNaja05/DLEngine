#include "GameObject.h"
#include <vector>

#include "../Components/Component.h"
#include "../DLCore/GastonMath.h"
#include "../DLCore/EventManager/EventManager.h"
#include "../DLCore/GastonData.h"

GameObject::~GameObject() {
    for (const auto& component : components_) {
        EventManager::Instance().StopListeningToAll(*component);
        component->Destroy();
    }
}

void GameObject::Destroy() {
    destroy_flag_ = true;
    for (const auto & child : children_) {
        child->Destroy();
    }
    for (const auto& child : add_queue_) {
        child->Destroy();
    }
}

void GameObject::CleanupDestroyedChildren() {
    std::erase_if(add_queue_, [](const std::unique_ptr<GameObject>& child) {
        return child->HasDestroyFlag();
    });
    std::erase_if(children_, [](const std::unique_ptr<GameObject>& child) {
        return child->HasDestroyFlag();
    });

    for (const auto & child : children_) {
        child->CleanupDestroyedChildren();
    }
}


bool GameObject::HasDestroyFlag() const {
    return destroy_flag_;
}

const Vector2& GameObject::Position() const {
    return transform_->position;
}

float GameObject::Rotation() const {
    return transform_->rotation;
}

float GameObject::Scale() const {
    return transform_->scale;
}

void GameObject::SetPosition(const Vector2& position) const {
    transform_->position = position;
    MarkTransformDirty();
}

void GameObject::Translate(const Vector2& translation) const {
    transform_->position += translation;
    MarkTransformDirty();
}

void GameObject::TranslateX(const float x) const {
    transform_->position.x += x;
    MarkTransformDirty();
}

void GameObject::TranslateY(const float y) const {
    transform_->position.y += y;
    MarkTransformDirty();
}

void GameObject::SetRotation(const float& rotation) const {
    transform_->rotation = rotation;
    MarkTransformDirty();
}

void GameObject::Rotate(const float &rotation) const {
    transform_->rotation += rotation;
    MarkTransformDirty();
}

void GameObject::SetScale(const float& scale) const {
    transform_->scale = scale;
    MarkTransformDirty();
}

void GameObject::ScaleBy(const float& scale) const {
    transform_->scale *= scale;
    MarkTransformDirty();
}

const Vector2& GameObject::GlobalPosition() const {
    SyncGlobalTransform();
    return transform_->global_position;
}

float GameObject::GlobalRotation() const {
    SyncGlobalTransform();
    return transform_->global_rotation;
}

float GameObject::GlobalScale() const {
    SyncGlobalTransform();
    return transform_->global_scale;
}

GameObject* GameObject::GetParent() const {
    return parent_;
}

void GameObject::AddChild(std::unique_ptr<GameObject> child) {
    for (const auto& c : children_) {
        if (c.get() == child.get()) return;
    }
    for (const auto& c : add_queue_) {
        if (c.get() == child.get()) return;
    }
    child->OnSetup(this);
    add_queue_.push_back(std::move(child));
}

std::unique_ptr<GameObject> GameObject::Create() {
    auto ptr =  std::unique_ptr<GameObject>(new GameObject());
    ptr->transform_ = std::make_unique<Transform2D>();
    return ptr;
}

std::vector<GameObject *> GameObject::GetChildren() const {
    std::vector<GameObject *> result;
    result.reserve(children_.size());

    for (const auto &c: children_)
        result.push_back(c.get());

    return result;
}

std::vector<GameObject *> GameObject::FindAllGameObjectsWithTag(const Tag tag) {
    std::vector<GameObject *> out;
    FindAllGameObjectsWithTagInternal(tag, out);
    return out;
}

void GameObject::FindAllGameObjectsWithTagInternal(const Tag tag, std::vector<GameObject *>& out) {
    if (HasTag(tag)) out.push_back(this);
    for (const auto& c : children_) {
        c->FindAllGameObjectsWithTagInternal(tag, out);
    }
}

void GameObject::AddChildrenInQueue() {
    for (auto& child : add_queue_) {
        children_.push_back(std::move(child));
    }
    add_queue_.clear();
    for (const auto & child : children_) {
        child->AddChildrenInQueue();
    }
}

void GameObject::OnSetup(GameObject* parent) {
    parent_ = parent;
    // Setup may append components. Indexing keeps vector reallocation safe
    // and gives each newly appended component its setup call as well.
    for (std::size_t i = 0; i < components_.size(); ++i)
        components_[i]->Setup();
    for (const auto& child : children_)
        child->OnSetup(this);
}

void GameObject::OnStart() {
    if (new_instance_flag_) {
        new_instance_flag_ = false;
        for (std::size_t i = 0; i < components_.size(); ++i)
            components_[i]->Start();
    }
    for (const auto& child : children_)
        child->OnStart();
}

void GameObject::OnUpdate(const float delta) const {
    if (destroy_flag_)
        return;
    for (const auto& component : components_)
        component->Update(delta);
    for (const auto& child : children_)
        child->OnUpdate(delta);
}

void GameObject::OnPhysics(const float delta) const {
    if (destroy_flag_)
        return;
    for (const auto& component : components_)
        component->PhysicsUpdate(delta);
    for (const auto& child : children_)
        child->OnPhysics(delta);
}

void GameObject::OnDraw() const {
    if (destroy_flag_)
        return;
    for (const auto& component : components_)
        component->QueueDraw();
    for (const auto& child : children_)
        child->OnDraw();
}

void GameObject::SyncGlobalTransform() const {
    if (!transform_->dirty) return;

    if (parent_) {
        parent_->SyncGlobalTransform();
        transform_->global_position = parent_->transform_->global_position +
            rotated(transform_->position, parent_->transform_->global_rotation)
            * parent_->transform_->global_scale;
        transform_->global_rotation = parent_->transform_->global_rotation + transform_->rotation;
        transform_->global_scale = parent_->transform_->global_scale * transform_->scale;
    } else {
        transform_->global_position = transform_->position;
        transform_->global_rotation = transform_->rotation;
        transform_->global_scale = transform_->scale;
    }
    transform_->dirty = false;
}

void GameObject::MarkTransformDirty() const {
    transform_->dirty = true;
    for (const auto & child : children_) {
        child->MarkTransformDirty();
    }
    for (const auto& child : add_queue_) {
        child->MarkTransformDirty();
    }
}
