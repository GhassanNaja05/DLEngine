#ifndef PILLAR_H
#define PILLAR_H
#include "../../ScriptComponent/Script.h"


class Pillar final : public Script {
public:
    Pillar() = default;
    ~Pillar() override = default;

    void Setup() override;
    void Update(float delta) override;
};



#endif
