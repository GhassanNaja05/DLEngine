#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H
#include "../../DLCore/EntityManager/EntityManager.h"
#include "../../DLCore/Modules/Timer/Timer.h"
#include "../EventListener/UserFacing.h"


class Script : public UserFacing {
protected:
    Script() = default;
    ~Script() override = default;

    [[nodiscard]] int CreateRepeatingTimer(float time, const std::function<void(Timer &)> &callback);
    void DestroyTimer(int id);
    void DestroyAllTimers();
    void CreateSingleShotTimer(float time, const std::function<void()> &callback);

    virtual void OnDestroy() {  }
private:
    std::unordered_map<int, Timer*> timers_;
    int timer_index_ = 0;

public:
    void Draw() const final {  }
    void PhysicsUpdate(float) final {  }

    void Destroy() final {
        DestroyAllTimers();
        OnDestroy();
    }


};



#endif
