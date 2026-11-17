#ifndef EVENTDATA_H
#define EVENTDATA_H
#include <cstdint>
#include <functional>

class Component;

/// Entry used by the event listener, when registering a new event
struct EventListenerEntry {
    Component* listener;
    std::function<void(Component &caller, const void *)> callback;
};

using Filter = std::function<bool(Component&)>;
using Tag = uint32_t;
/// Describes a filter that can be applied to Components
struct Target {
    /// Function that can be applied to filter between targets
    Filter filter_ = [] (Component&) { return true; };

    /// Select all targets
    static Target All();

    /// Select no targets
    static Target None();

    /// Select targets with the given tag
    static Target HasTag(Tag tag);
};

#endif
