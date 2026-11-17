#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "../Component.h"
#include "../../DLCore/Engine/Engine.h"


class Drawable : public Component {
public:
    ~Drawable() override = default;

    void QueueDraw() const final {
        Engine::Instance().EnterRenderQueue(this, render_layer_);
    }

    void Draw() const override = 0;

    int& Layer() { return render_layer_; }

protected:

    Drawable() = default;

    int render_layer_ = 0;
};



#endif
