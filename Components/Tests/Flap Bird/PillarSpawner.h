#ifndef PILLARSPAWNER_H
#define PILLARSPAWNER_H
#include "../../ScriptComponent/Script.h"


class PillarSpawner final : public Script {
public:
    PillarSpawner() = default;
    ~PillarSpawner() override = default;

    void Update(float delta) override;

private:
    float accumulator = 0.0f;
    const float next = 10.0f;
    float next_pillar = 10.0f;
    float previous_x = 0.0f;
};



#endif
