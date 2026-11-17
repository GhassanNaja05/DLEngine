#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <vector>

#include "../../GameObject/GameObject.h"
#include "../GastonData.h"


template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

class EntityManager {

public:
    /// Returns the EntityManager singleton
    static EntityManager& Instance();

    CameraConfig& Camera() { return camera_; }

    /// Returns the root GameObject
    [[nodiscard]] GameObject& GetRoot() const {
        return *root_;
    }

    ///Initialize the root
    void InitRoot() {
        root_.reset();
        camera_ = {};
        root_ = GameObject::Create();
        root_->OnSetup(nullptr);
    }

    /// Adds an empty child to the provided parent and returns a reference to it
    static GameObject& CreateEmptyChild(GameObject* parent);

    /// Creates a new child for the provided parent and adds the provided components to it.
    template<IsComponent... Components>
    static GameObject& CreateChild(GameObject* parent) {
        auto child = std::make_unique<GameObject>();
        GameObject* raw = child.get();

        parent->AddChild(std::move(child));
        (raw->AddComponent<Components>(), ...);

        return *raw;
    }

    /// Starts the clean-up phase to delete objects flagged for destruction
    void CleanupDestroyed() const;

    void DestroyAllChildren() const;
    void Shutdown() { root_.reset(); }

private:
    CameraConfig camera_ {  };

    EntityManager() = default;

    std::unique_ptr<GameObject> root_ = nullptr;
};

#endif
