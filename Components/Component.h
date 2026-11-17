#ifndef COMPONENT_H
#define COMPONENT_H
#include <assert.h>

struct CameraConfig;
namespace implementation {
    class GameObjectAccess;
}
class GameObject;
class Component {

public:
    virtual ~Component() = default;

    /// Returns a reference to this component's owner
    [[nodiscard]] GameObject* Owner() const { return owner_; }
protected:

    /// Called upon the creation of the object
    virtual void Setup() {

    }

    /// Called once before Update is called for the first time
    virtual void Start() {
    }

    /// Called at every frame, once Start has been called once
    virtual void Update(float delta) {

    }

    /// Called at every frame, once Start has been called and after Update
    virtual void PhysicsUpdate(float delta) {

    }

    /// Called at every fame, once Start has been called, after PhysicsUpdate, used to draw (render) the object
    virtual void Draw() const {

    }

    virtual void RenderCanvas() const {
        assert(false);
    }

    /// Called at every fame, once Start has been called, after PhysicsUpdate, used to draw (render) the object
    virtual void QueueDraw() const {

    }

    explicit Component() = default;

private:

    /// Called when the owner of this component has requested destruction
    virtual void Destroy() {

    }

    GameObject* owner_ = nullptr;

    friend class GameObject;
    friend class Engine;

};



#endif
