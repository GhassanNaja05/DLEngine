#ifndef EVENTUSER_H
#define EVENTUSER_H
#include "../Component.h"
#include "../../DLCore/EventManager/EventManager.h"
#include "../../DLCore/EntityManager/EntityManager.h"

class UserFacing : public Component {
protected:
    UserFacing() = default;

public:
    ~UserFacing() override = default;

    /// Listen to a specific event and specify its callback method
    template<typename T>
    void Listen(std::function<void(Component &caller, const T &event)> callback) {
        event_manager_.Listen(*this, callback);
    }

    /// Emit an event and specify its targets
    template<typename T>
    void Emit(const T& event, const Target& target) {
        event_manager_.Emit(*this, event, target);
    }

    /// Emit an event
    template<typename T>
    void Emit(const T& event) {
        event_manager_.Emit(*this, event);
    }

    /// Stop listening to a specific event
    template<typename T>
    void StopListening() {
        event_manager_.StopListening<T>(*this);
    }

    /// Stop listening to all events
    void StopListeningToAll() const {
        event_manager_.StopListeningToAll(*this);
    }

    /// Get a reference to the scene camera
    [[nodiscard]] virtual CameraConfig& GetCamera() const final {
        return EntityManager::Instance().Camera();
    }

    /// Get a reference to the root GameObject
    [[nodiscard]] virtual GameObject& GetRoot() const final {
        return EntityManager::Instance().GetRoot();
    }

private:
    EventManager& event_manager_ = EventManager::Instance();

};

#endif
