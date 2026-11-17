#include "Script.h"
#include <functional>

int Script::CreateRepeatingTimer(const float time, const std::function<void(Timer &)> &callback) {
    auto* timer = Timer::CreateTimer(time, callback, true);
    timers_.insert({ timer_index_, timer });
    return timer_index_++;
}

void Script::DestroyTimer(const int id) {
    if (const auto t = timers_.find(id); t != timers_.end()) {
        Timer::DestroyTimer(t->second);
        timers_.erase(t);
    }
}

void Script::DestroyAllTimers() {
    for (auto& t : timers_) {
        Timer::DestroyTimer(t.second);
    }
    timers_.clear();
}

void Script::CreateSingleShotTimer(const float time, const std::function<void()> &callback) {
    int id = timer_index_;
    auto* t = Timer::CreateTimer(time, [callback, this, id] (Timer& timer) {
        callback();
        this->timers_.erase(id);
    }, false);
    timers_.insert({ timer_index_++, t });
}
