#ifndef DOUDOULAPIN_H
#define DOUDOULAPIN_H
#include "../../ScriptComponent/Script.h"


class PhysicsBody;

class DoudouLapin final : public Script {

public:
    DoudouLapin() = default;
    ~DoudouLapin() override = default;

    void Setup() override;
    void Update(float delta) override;

private:
    PhysicsBody* p = nullptr;
    // Grounded, jumping, falling, wall contact, wall jump.
    int state = 2;
    bool input_cooldown = false;
    bool was_cornered = false;
    float last_snap = 0.0f;

};



#endif
