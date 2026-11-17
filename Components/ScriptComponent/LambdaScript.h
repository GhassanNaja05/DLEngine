#ifndef LAMBDASCRIPT_H
#define LAMBDASCRIPT_H
#include "Script.h"
#include <functional>

class LambdaScript final : public Script {
public:
    LambdaScript() = default;
    ~LambdaScript() override = default;

    void Start() override  { OnStart(*this); }
    void Update(const float delta) override { OnUpdate(*this, delta); }

    /// Override this component's Start method
    void OverrideStart(const std::function<void(LambdaScript&)>& on_start) {
        OnStart = on_start;
    }

    /// Override this component's Update method
    void OverrideUpdate(const std::function<void(LambdaScript&, float)>& on_update) {
        OnUpdate = on_update;
    }

private:

    std::function<void(LambdaScript&)> OnStart = [](auto&) {  };
    std::function<void(LambdaScript&, float)> OnUpdate = [](auto&, float) {  };
};

#endif
