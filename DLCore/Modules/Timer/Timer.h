#ifndef TIMER_H
#define TIMER_H
#include <functional>
#include <memory>
#include <vector>


class Timer final {
protected:
    Timer() = default;

    static void UpdateTimers(float delta);
    static void Clear() { timers_.clear(); add_queue_.clear(); }

    static inline auto timers_ = std::vector<std::unique_ptr<Timer>>();
    static inline auto add_queue_ = std::vector<std::unique_ptr<Timer>>();

    [[nodiscard]] static Timer* CreateTimer(float time, const std::function<void(Timer&)>& callback, bool is_repeating = false);

    static void DestroyTimer(const Timer * target);


    std::function<void(Timer&)> callback_;
    float time_remaining_ = -.1f;
    float time_total_ = -.1f;
    bool destroy_on_done_ = true;
    bool ready_to_destroy_ = false;

    void RequestDestroy() const;

public:
    void SetTime(float time);
    [[nodiscard]] float GetTotalTime() const;
    [[nodiscard]] float GetTimeRemaining() const;


    friend class Script;
    friend class Engine;
    ~Timer() = default;

};



#endif
