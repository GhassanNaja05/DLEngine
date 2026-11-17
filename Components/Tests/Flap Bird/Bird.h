#ifndef BIRD_H
#define BIRD_H
#include "../../PhysicsBody/PhysicsBody.h"
#include "../../ScriptComponent/Script.h"
#include "../../Shape/RectangleShape.h"
#include "../../Sprites/RectangleImage.h"


class Bird final : public Script {
public:
    Bird() = default;
    ~Bird() override = default;

    void Setup() override;
    void Update(float delta) override;
};



#endif
