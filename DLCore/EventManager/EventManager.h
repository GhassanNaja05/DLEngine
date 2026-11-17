#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <algorithm>
#include <unordered_map>
#include <vector>


#include "../GastonData.h"
#include "EventData.h"


struct Target;
class Component;
class EventManager {

    using EventType = void*;
    using Listeners = std::unordered_map<EventType, std::vector<EventListenerEntry>>;

public:
    EventManager() = default;
    ~EventManager() = default;

    /// Return the EventManager singleton
    static EventManager& Instance() {
        static EventManager instance;
        return instance;
    }

    /// Registers a Component as a Listener to event of type T. Allows to specify the callback method
    template<typename T>
    void Listen(Component& listener, std::function<void (Component& caller, const T& event)> callback) {
        auto type = GetEventTypeId<T>();

        StopListening<T>(listener);

        listeners_[type].push_back({
            &listener,
            [callback] (Component& caller, const void* event) {
                callback(caller, *static_cast<const T*>(event)) ;
            }
        });
    }

    /// Emits an event T to all specified targets
    template<typename T>
    void Emit(Component& caller, const T& event, const Target& target) {
        auto type = GetEventTypeId<T>();

        auto it = listeners_.find(type);
        if (it == listeners_.end())
            return;

        for (const auto& entry : it->second) {
            if (!target.filter_(*entry.listener)) continue;
            entry.callback(caller, &event);
        }
    }

    /// Emits an event T to all listeners
    template<typename T>
    void Emit(Component& caller, const T& event) {
        Emit(caller, event, Target::All());
    }

    /// Unregisters a listeners from listening to an event T
    template<typename T>
    void StopListening(Component& listener) {
        auto type = GetEventTypeId<T>();
        auto it = listeners_.find(type);

        if (it == listeners_.end())
            return;

        auto& vec = it->second;

        std::erase_if(vec, [&](const EventListenerEntry& entry) {
            return entry.listener == &listener;
        });

        if (vec.empty()) {
            listeners_.erase(it);
        }
    }

    /// Stop the listener from listening to all events
    void StopListeningToAll(const Component& listener);

private:
    template<typename>
    [[nodiscard]] static EventType GetEventTypeId() {
        static int unique;
        return &unique;
    }

    Listeners listeners_ = Listeners();
};
#endif
