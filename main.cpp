#include <raylib.h>

#ifdef DLENGINE_FLAP_BIRD
#include "Components/Tests/Flap Bird/FlapBirdMaker.h"
#else
#include "Components/Tests/WallJump/WallJumpMaker.h"
#endif
#include "DLCore/Engine/Engine.h"
#include "DLCore/EntityManager/EntityManager.h"

namespace {
void CreateDemo() {
    auto& root = EntityManager::Instance().GetRoot();
#ifdef DLENGINE_FLAP_BIRD
    (void)root.CreateChild<FlapBirdMaker>();
#else
    (void)root.CreateChild<WallJumpMaker>();
#endif
}
}

int main() {
    auto& engine = Engine::Instance();
    engine.Init();
    if (!IsWindowReady()) return 1;

#ifdef DLENGINE_FLAP_BIRD
    engine.SetDemoTitle("Flap Bird", "SPACE flap   |   R restart   |   ESC quit");
#else
    engine.SetDemoTitle("WallJump", "A/D or arrow keys move   |   SPACE jump / wall jump   |   R restart   |   ESC quit");
#endif
    CreateDemo();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            EntityManager::Instance().InitRoot();
            CreateDemo();
        }
        engine.Tick(GetFrameTime());
    }

    engine.Shutdown();
    return 0;
}
