#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <cassert>
#include <vector>

#include <raylib.h>


#include "../Components/Component.h"
#include "../DLCore/Debug/Debug.h"

struct Transform2D;

class EntityManager;
class GameObject final {

public:

    using Tag = uint32_t;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;
    ~GameObject();

    /// Returns a vector representing the relative position
    [[nodiscard]] const Vector2& Position() const;

    /// Returns a float representing the relative rotation, in degrees
    [[nodiscard]] float Rotation() const;

    /// Returns a float representing the relative scale
    [[nodiscard]] float Scale() const;


    /// Sets the object position to a specific value
    void SetPosition(const Vector2& position) const;

    /// Moves the object along the x and y-axis by a specified value
    void Translate(const Vector2& translation) const;

    /// Moves the object along the x-axis by a specified amount
    void TranslateX(float x) const;

    /// Moves the object along the y-axis by a specified amount
    void TranslateY(float y) const;


    /// Sets the rotation to a specified value, in degrees
    void SetRotation(const float& rotation) const;

    /// Rotates the object by a specific amount, in degrees
    void Rotate(const float& rotation) const;


    /// Sets the scale to a specified value
    void SetScale(const float& scale) const;

    /// Multiplies the scale by a specified value
    void ScaleBy(const float& scale) const;


    /// Returns the objects global position, relative to the world origin
    [[nodiscard]] const Vector2& GlobalPosition() const;

    /// Returns the object's global rotation, relative to the world origin
    [[nodiscard]] float GlobalRotation() const;

    /// Returns the object's global scale, relative to the world origin
    [[nodiscard]] float GlobalScale() const;


    /// Returns a pointer to the object's parent
    [[nodiscard]] GameObject* GetParent() const;

    /// Return the first child that has the matching component
    template<class T>
    [[nodiscard]] GameObject* GetChildWithComponent() const {
        for (const auto& child : children_) {
            if (child->GetComponent<T>() != nullptr)
                return child.get();
        }
        return nullptr;
    }

    /// Return a reference to all children already added to the object
    [[nodiscard]] std::vector<GameObject*> GetChildren() const;

    /// Returns the first of the specified component, or nullptr if not found
    template<typename T>
    [[nodiscard]] T* GetComponent() const {
        for (const auto& component : components_) {
            if (auto casted = dynamic_cast<T*>(component.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    /// Adds a component of the specified type to the object
    template<typename T>
    [[nodiscard]] T& AddComponent() {
        if (!std::is_base_of_v<Component, T>) {
            throw std::runtime_error("AddComponent called on non-component type");
        }

        auto component = std::make_unique<T>();
        component->owner_ = this;

        T &ref = *component;
        components_.push_back(std::move(component));
        return ref;
    }

    /// Create and add a child with no components
    [[nodiscard]] GameObject& CreateEmptyChild();

    /// Create and add a child with the provided components
    template<typename... Components>
    [[nodiscard]] GameObject& CreateChild();

    template<typename... Components, typename Function>
    [[nodiscard]] GameObject& CreateChild(Function setup);

    /// Add the given tag to the GameObject
    void AddTag(const Tag tag) {
        assert(IsPowerOfTwo(tag) && "Tag must be a single bit!!!");
        tags_ |= tag;
    }

    /// Add the given tags to the GameObject
    template<typename... Tags>
    void AddTags(Tags... tags) {
        (AddTag(tags), ...);
    }

    /// Remove the given tag from the GameObject
    void RemoveTag(const Tag tag) {
        assert(IsPowerOfTwo(tag) && "Tag must be a single bit!!!");
        tags_ &= ~tag;
    }

    /// Remove the given tags from the GameObject
    template<typename... Tags>
    void RemoveTags(Tags... tags) {
        (RemoveTag(tags), ...);
    }

    /// Toggle the given tag
    void ToggleTag(const Tag tag) {
        assert(IsPowerOfTwo(tag) && "Tag must be a single bit!!!");
        tags_ ^= tag;
    }

    /// Remove all tags from the GameObject
    void ClearTags() {
        tags_ = 0;
    }

    [[nodiscard]] bool HasTags() const {
        return tags_ != 0;
    }

    /// Checks if the GameObject has the given tag
    [[nodiscard]] bool HasTag(const Tag tag) const {
        return (tag & tags_) != 0;
    }

    /// Checks if the GameObject has all the provided tags
    [[nodiscard]] bool HasTags(const Tag tag) const {
        return HasTag(tag);
    }

    /// Checks if the GameObject has all the provided tags
    template<typename... Tags>
    [[nodiscard]] bool HasTags(const Tag first, Tags... tags) {
        return HasTag(first) && HasTags(tags...);
    }

    /// Checks if the GameObject has any tags
    [[nodiscard]] bool HasAnyTags() const {
        return tags_ != 0;
    }

    /// Checks if the GameObject has at least one of the provided tags
    [[nodiscard]] bool HasAnyTags(const Tag tag) const {
        return HasTag(tag);
    }

    /// Checks if the GameObject has at least one of the provided tags
    template<typename... Tags>
    [[nodiscard]] bool HasAnyTags(const Tag first, Tags... tags) {
        return HasTag(first) || HasTags(tags...);
    }

    /// Return the integer value of the tag mask
    [[nodiscard]] Tag GetTagMask() const {
        return tags_;
    }

    /// Return the first object with a given tag
    [[nodiscard]] GameObject* FindFirstGameObjectWithTag(const Tag tag) {
        if (HasTags(tag)) {
            return this;
        }
        for (const auto& child : children_) {
            if (auto* ptr = child->FindFirstGameObjectWithTag(tag))
                return ptr;
        }
        return nullptr;
    }

    [[nodiscard]] std::vector<GameObject*> FindAllGameObjectsWithTag(Tag tag);

    /// Request this object's destruction
    void Destroy();

protected:
    GameObject() = default;

private:

    uint32_t tags_ = 0;

    /// Checks if a tag is a power of 2
    static constexpr bool IsPowerOfTwo(const Tag t) {
        return t && (t & t - 1) == 0;
    }

    /// Internal helper method to create a child and add it to this GameObject
    static GameObject& CreateChildInternal(
        GameObject& parent,
        void (*add_components)(GameObject&)
    );

    void FindAllGameObjectsWithTagInternal(Tag tag, std::vector<GameObject*>& out);

    /// Internal helper method to create an empty child
    static GameObject& CreateEmptyChildInternal(GameObject& parent);

    mutable std::unique_ptr<Transform2D> transform_ = nullptr;

    bool new_instance_flag_ = true;
    bool destroy_flag_ = false;

    GameObject* parent_ = nullptr;
    std::vector<std::unique_ptr<GameObject>> children_;
    std::vector<std::unique_ptr<GameObject>> add_queue_;

    /// Add all the children in the queue
    void AddChildrenInQueue();

    std::vector<std::unique_ptr<Component>> components_;

    /// Returns number of components of type T this object has
    template<typename T>
    [[nodiscard]] std::size_t CountComponents() const {
        std::size_t count = 0;
        for (const auto& c : components_) {
            if (dynamic_cast<T*>(c.get()))
                ++count;
        }
        return count;
    }

    /// Recalculates global transform, if marked dirty
    void SyncGlobalTransform() const;

    /// Marks the global transform as dirty (needs to be recalculated)
    void MarkTransformDirty() const;

    /// Initiates the setup phase on the object
    void OnSetup(GameObject* parent);

    /// Initiates the start phase on the object
    void OnStart();

    /// Initiates the update phase on the object
    void OnUpdate(float delta) const;

    /// Initiates the physics phase on the object
    void OnPhysics(float delta) const;

    /// Initiates the draw phase on the object
    void OnDraw() const;

    /// Returns true if the object has the destroy flag, false otherwise
    [[nodiscard]] bool HasDestroyFlag() const;

    /// Cleans up destroyed children, if any have requested destruction
    void CleanupDestroyedChildren();

    /// Adds the provided GameObject to this object's list of children on the next frame
    void AddChild(std::unique_ptr<GameObject> child);

    /// Creates a unique GameObject
    static std::unique_ptr<GameObject> Create();

    friend class EntityManager;
    friend class Engine;
};

template<typename... Components>
GameObject& GameObject::CreateChild() {
    return CreateChildInternal(*this, [](GameObject &child) { ((void) child.AddComponent<Components>(), ...); });
}

template<typename... Components, typename Function>
GameObject& GameObject::CreateChild(Function setup) {
    auto& new_child =  CreateChild<Components...>();
    setup(new_child, (new_child.template GetComponent<Components>())...);

    return new_child;
}

inline GameObject& GameObject::CreateEmptyChild() {
    return CreateEmptyChildInternal(*this);
}



#endif
