#include "Timer.h"

#include "../../Debug/Debug.h"

void Timer::UpdateTimers(const float delta) {
    for (auto& timer : timers_) {
        if (timer->ready_to_destroy_ || timer->time_remaining_ <= 0.0f) continue;
        timer->time_remaining_ -= delta;
        if (timer->time_remaining_ <= 0.0f) {
            timer->callback_(*timer);
            timer->time_remaining_ = timer->time_total_;
            if (timer->destroy_on_done_) timer->ready_to_destroy_ = true;
        }
    }
    std::erase_if(timers_, [] (const std::unique_ptr<Timer> &timer) { return timer->ready_to_destroy_; });
    for (auto& timer : add_queue_) {
        if (!timer->ready_to_destroy_) timers_.push_back(std::move(timer));
    }
    add_queue_.clear();
}

Timer* Timer::CreateTimer(const float time, const std::function<void(Timer &)>& callback, const bool is_repeating) {
    add_queue_.push_back(std::unique_ptr<Timer>(new Timer()));
    add_queue_.back()->callback_ = callback;
    add_queue_.back()->SetTime(time);
    add_queue_.back()->destroy_on_done_ = !is_repeating;

    return add_queue_.back().get();
}


void Timer::SetTime(const float time) {
    if (time <= 0.0f) {
        time_total_ = -.1f;
        time_remaining_ = time_total_;
    }
    else {
        time_total_ = time;
        time_remaining_ = time_total_;
    }
}

float Timer::GetTotalTime() const {
    return time_total_;
}

float Timer::GetTimeRemaining() const { return time_remaining_; }

void Timer::DestroyTimer(const Timer *target) {
    for (auto& timer : timers_) {
        if (timer.get() == target) {
            timer->ready_to_destroy_ = true;
        }
    }
    for (auto& timer : add_queue_) {
        if (timer.get() == target) timer->ready_to_destroy_ = true;
    }
}

void Timer::RequestDestroy() const {
    DestroyTimer(this);
}
