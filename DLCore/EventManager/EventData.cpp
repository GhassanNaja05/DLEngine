#include "EventData.h"
#include "../../Components/Component.h"
#include "../../GameObject/GameObject.h"

Target Target::All() {
    return Target { [] (Component&) { return true; } };
}

Target Target::None() {
    return Target { [] (Component&) { return false; } };
}

Target Target::HasTag(Tag tag) {
    return Target { [tag] (const Component& c) {
        return c.Owner()->HasTag(tag);
    }};
}
