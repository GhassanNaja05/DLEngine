#ifndef FLAPBIRDMAKER_H
#define FLAPBIRDMAKER_H
#include "../../Component.h"
#include "../../ScriptComponent/Script.h"


class FlapBirdMaker final : public Script {
public:
    FlapBirdMaker() = default;
    ~FlapBirdMaker() override = default;

    void Setup() override;
};



#endif
