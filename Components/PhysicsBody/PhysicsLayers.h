#ifndef PHYSICSLAYERS_H
#define PHYSICSLAYERS_H

typedef int Layer;

struct PhysicsLayers {
    PhysicsLayers() = delete;
    ~PhysicsLayers() = delete;

    static constexpr Layer Player = 1 << 0;
    static constexpr Layer Enemy = 1 << 1;
    static constexpr Layer Wall = 1 << 2;
    static constexpr Layer Projectile = 1 << 3;
    static constexpr Layer Floor = 1 << 4;
};

#endif
