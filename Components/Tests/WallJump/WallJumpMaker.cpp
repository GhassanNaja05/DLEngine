#include "WallJumpMaker.h"

#include "../../PhysicsBody/PhysicsBody.h"
#include "../../PhysicsBody/PhysicsLayers.h"
#include "../../ScriptComponent/LambdaScript.h"
#include "../../Shape/RectangleShape.h"
#include "../../Sprites/RectangleImage.h"
#include "DoudouLapin.h"

void WallJumpMaker::Setup() {

    auto& owner = *Owner();

    // --- 1. THE MAIN FLOOR ---
    (void) owner.CreateChild<RectangleImage, RectangleShape, PhysicsBody>([]
        (auto& owner, auto* image, auto* shape, auto* body) {
        owner.SetPosition({0.0f, 15.0f});
        image->RectColor() = {40, 40, 40, 255}; // Dark Floor
        image->Width() = 60.0f;
        image->Height() = 2.0f;
        shape->SetupFromRectangleImage();
        body->RegisterWithRectangleShape();
        body->SetStatic(true);
        body->Friction() = 0.0f;
        body->ExcludeAllLayers();
        body->ToggleExcludeLayer(PhysicsLayers::Player);
        body->ToggleLayer(PhysicsLayers::Wall);
    });

    // --- 2. THE LEFT "CLIMBING" TOWER ---
    // A tall wall to test long slides and the initial jump
    (void) owner.CreateChild<RectangleImage, RectangleShape, PhysicsBody>([]
        (auto& owner, auto* image, auto* shape, auto* body) {
        owner.SetPosition({-15.0f, 0.0f});
        image->RectColor() = RED;
        image->Width() = 2.0f;
        image->Height() = 30.0f;
        shape->SetupFromRectangleImage();
        body->RegisterWithRectangleShape();
        body->SetStatic(true);
        body->Friction() = 0.0f;
        body->ExcludeAllLayers();
        body->ToggleExcludeLayer(PhysicsLayers::Player);
        body->ToggleLayer(PhysicsLayers::Wall);
    });

    // --- 3. FLOATING ZIG-ZAGS (The "Twists") ---
    // Staggered platforms to test jumping from wall to ledge
    auto addPlatform = [&](Vector2 pos, Vector2 size, Color col) {
        (void) owner.CreateChild<RectangleImage, RectangleShape, PhysicsBody>([pos, size, col]
            (auto& owner, auto* image, auto* shape, auto* body) {
            owner.SetPosition(pos);
            image->RectColor() = col;
            image->Width() = size.x;
            image->Height() = size.y;
            shape->SetupFromRectangleImage();
            body->RegisterWithRectangleShape();
            body->SetStatic(true);
            body->Friction() = 0.0f;
            body->ExcludeAllLayers();
            body->ToggleExcludeLayer(PhysicsLayers::Player);
            body->ToggleLayer(PhysicsLayers::Wall);
        });
    };

    // Low platform to get started
    addPlatform({-8.0f, 8.0f}, {6.0f, 1.0f}, BLUE);

    // Mid-air "C" turn: A wall on the right to bounce back from
    addPlatform({5.0f, 5.0f}, {2.0f, 10.0f}, BLACK);

    // --- 4. THE "CHIMNEY" (Testing the 0.25s lock) ---
    // Two narrow walls for rapid vertical climbing
    // Gap is 5 units. Player is 2 units. 1.5 units padding on each side.
    addPlatform({15.0f, -5.0f}, {2.0f, 20.0f}, PURPLE); // Left chimney wall
    addPlatform({20.0f, -5.0f}, {2.0f, 20.0f}, PURPLE); // Right chimney wall

    // --- 5. THE OVERHANG ---
    // Test for "unsnapping" when you hit a ceiling/corner
    addPlatform({5.0f, -10.0f}, {10.0f, 2.0f}, GREEN);

    // --- 6. THE PLAYER ---
    (void) owner.CreateChild<DoudouLapin>();
}
