#include "Engine.h"

#include "../../GameObject/GameObject.h"
#include "../Debug/Debug.h"
#include "../EntityManager/EntityManager.h"
#include "../GastonMath.h"
#include "../Modules/Audio/Audio.h"
#include "../Modules/Input/Input.h"
#include "../Modules/SpriteManager/SpriteManager.h"
#include "../Modules/Timer/Timer.h"
#include "../PhysicsManager/PhysicsManager.h"

Engine &Engine::Instance() {
    static Engine instance;
    return instance;
}

void Engine::Init() {

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);

    if constexpr (FULL_SCREEN) {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(SCREEN_FPS);

    if constexpr (!FULL_SCREEN) {
        InitWindow(SCREEN_X, SCREEN_Y, TITLE);
    }
    else {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    }
    InitAudioDevice();

    if (HIDE_CURSOR) HideCursor();

    CalculateUnitScale();

    EntityManager::Instance().InitRoot();
}

void Engine::SetDemoTitle(const std::string& title, const std::string& controls) {
    demo_title_ = title;
    demo_controls_ = controls;
    SetWindowTitle((title + " | DLEngine").c_str());
}

void Engine::Shutdown() {
    EntityManager::Instance().Shutdown();
    Timer::Clear();
    Audio::Instance().Clear();
    SpriteManager::Instance().Clear();
    if (IsAudioDeviceReady()) CloseAudioDevice();
    if (IsWindowReady()) CloseWindow();
}

void Engine::Tick(const float delta) {

    Audio::Instance().Update();

    GameObject& root = EntityManager::Instance().GetRoot();

    // Add children in the add queue at the start of each frame
    root.AddChildrenInQueue();

    // Start phase, for newly created GameObjects
    root.OnStart();

    Timer::UpdateTimers(delta);
    // Update and Physics phase
    delta_accumulator_ += delta;
    const auto& physics_manager = PhysicsManager::Instance();
    int frames = 0;
    while (delta_accumulator_ >= frame_length_) {
        Input::Update(frame_length_);
        root.OnUpdate(frame_length_);
        physics_manager.Step(frame_length_);
        root.OnPhysics(frame_length_);
        EntityManager::Instance().CleanupDestroyed();

        delta_accumulator_ -= frame_length_;
        if (++frames >= 8)
            delta_accumulator_ = 0.0f;
    }

    // Drawing phase starts, separated into world and UI
    RenderScene();
    // Drawing phase ends

    // Cleanup phase, for objects marked for destruction
    EntityManager::Instance().CleanupDestroyed();

}

void Engine::RenderScene() {
    render_queue_.clear();

    const GameObject& root = EntityManager::Instance().GetRoot();
    const auto& camera = EntityManager::Instance().Camera();

    BeginDrawing();

    BeginMode2D(
        Camera2D {
            {
                HalfScreenX(),
                HalfScreenY()
            },
            camera.target * true_unit_size,
            camera.tilt,
            camera.zoom
        });

    ClearBackground(RAYWHITE);

    root.OnDraw();

    std::ranges::sort(render_queue_, [] (const RenderQueueEntry & a, const RenderQueueEntry & b) {
        return a.layer < b.layer;
    });

    for (auto &[layer, component] : render_queue_) {
        component->Draw();
    }

    EndMode2D();

    if (SHOW_GRID)
        DrawGrid();

    if (!demo_title_.empty()) {
        DrawRectangle(0, 0, GetScreenWidth(), 64, Color{25, 30, 42, 240});
        DrawText(demo_title_.c_str(), 20, 12, 20, RAYWHITE);
        DrawText(demo_controls_.c_str(), 20, 39, 16, Color{190, 203, 220, 255});
    }

    EndDrawing();

}

void Engine::CalculateUnitScale() {
    unit_scale = static_cast<float>(GetScreenHeight()) / static_cast<float>(SCREEN_Y);
    true_unit_size = unit_scale * UNIT_SIZE;
}

void Engine::DrawLine(const float length, const float thickness, const float angle, const Vector2 &center,
                      const Color &color) {
    const auto rec = Rectangle{center.x, center.y, length, thickness};
    DrawRectanglePro(rec, Vector2{length / 2.0f, thickness / 2.0f}, angle, color);
}

float Engine::GetScreenUnitSizeX() const {
    return ScreenX() / true_unit_size;
}

float Engine::GetScreenUnitSizeY() const {
    return ScreenY() / true_unit_size;
}

void Engine::DrawGrid() const {
    constexpr auto grayish = Color{ 0, 0, 0, 31 };
    constexpr auto blackish = Color{ 0, 0, 0, 63 };

    const auto [position, tilt, zoom] = EntityManager::Instance().Camera();
    const float unit_size = true_unit_size * zoom;
    const float scaled_unit = unit_size;

    const auto rotate = get_rotator(tilt);
    const auto rotate_90 = get_rotator(tilt + 90.0f);

    constexpr float line_thickness = 1.5f;
    const float length = std::max(ScreenX(), ScreenY()) * 1.5f;

    const auto offset = Vector2{ ScreenX(), ScreenY() } / 2.0f;
    const auto relative_origin = rotate(-position * unit_size);

    const auto rounded_camera_pos_x = rotate(Vector2{
        std::round(position.x) * scaled_unit,
        0.0f
    });

    const auto rounded_camera_pos_y = rotate(Vector2{
        0.0f,
        std::round(position.y) * scaled_unit
    });

    const auto rounded_camera_pos = rounded_camera_pos_x + rounded_camera_pos_y;

    const auto absolute_position = offset + rounded_camera_pos + relative_origin;

    const float target_position = std::floor(length / scaled_unit) * scaled_unit;
    float xy_position = -target_position;
    const float tilt_90 = tilt + 90.0f;
    constexpr float line_thickness_125 = line_thickness * 1.25f;

    while (xy_position < target_position) {
        const auto h_rotated = absolute_position + rotate(Vector2{ 0.0f, xy_position });
        const auto v_rotated = absolute_position + rotate_90(Vector2{ 0.0f, xy_position });

        DrawLine(
            length,
            line_thickness,
            tilt,
            h_rotated,
            grayish);

        DrawLine(
            length,
            line_thickness,
            tilt_90,
            v_rotated,
            grayish);

        xy_position += scaled_unit;
    }

    DrawLine(
        length,
        line_thickness,
        tilt,
        offset + relative_origin + rounded_camera_pos_x,
        blackish);

    DrawLine(
        length,
        line_thickness_125,
        tilt_90,
        offset + relative_origin + rounded_camera_pos_y,
        blackish);
}
