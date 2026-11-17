#include "EventManager.h"
#include "../GastonData.h"

void EventManager::StopListeningToAll(const Component& listener) {
    for (auto it = listeners_.begin(); it != listeners_.end(); ) {
        auto& vec = it->second;
        std::erase_if(vec, [&](const EventListenerEntry& e) {
            return e.listener == &listener;
        });

        if (vec.empty())
            it = listeners_.erase(it);
        else
            ++it;
    }
}
