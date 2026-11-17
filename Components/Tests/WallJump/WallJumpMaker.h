#ifndef WALLJUMPMAKER_H
#define WALLJUMPMAKER_H
#include "../../ScriptComponent/Script.h"


class WallJumpMaker final : public Script{

public:
    WallJumpMaker() = default;
    ~WallJumpMaker() override = default;

    void Setup() override;

};



#endif
