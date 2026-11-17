#ifndef ENGINE_H
#define ENGINE_H
#include <algorithm>
#include <string>
#include <vector>
#include "../GastonData.h"


class Engine {
public:

    /// Get the screen width in units
    [[nodiscard]] float GetScreenUnitSizeX() const;

    /// Get the screen height in units
    [[nodiscard]] float GetScreenUnitSizeY() const;

    static constexpr int SCREEN_WIDTH = 2560;
    static constexpr int SCREEN_HEIGHT = 1600;

    static constexpr float UNIT_SIZE = 30.0f;

    static constexpr float SCREEN_WIDTH_F = 2560.0f;
    static constexpr float SCREEN_HEIGHT_F = 1600.0f;

    static constexpr bool FULL_SCREEN = false;
    static constexpr bool HIDE_CURSOR = false;

    static constexpr int SCREEN_X = 1000;
    static constexpr int SCREEN_Y = 600;

    static constexpr float SCREEN_X_F = 1000.0f;
    static constexpr float SCREEN_Y_F = 600.0f;

    static constexpr float SCREEN_UNITS_X = SCREEN_X_F / UNIT_SIZE;
    static constexpr float SCREEN_UNITS_Y = SCREEN_Y_F / UNIT_SIZE;

    static constexpr float FPS = 240.0f;
    static constexpr int SCREEN_FPS = static_cast<int>(FPS / 2.0f);
    static constexpr float frame_length_ = 1.0f / FPS;

    static constexpr const char* TITLE = "DLEngine";

    static constexpr bool DEBUG_MODE = false;
    static constexpr bool SHOW_GRID = false;

    static constexpr int PHYSICS_PASSES = 1;

    float true_unit_size = 1.0f;
    float unit_scale = 1.0f;

    /// Returns the Engine singleton
    static Engine& Instance();
    void CalculateUnitScale();

    /// Initialize, runs at the beginning of the game
    void Init();
    void Shutdown();
    void SetDemoTitle(const std::string& title, const std::string& controls);

    /// Runs every frame, represents one in-game frame
    void Tick(float delta);

    static float ScreenX() { return static_cast<float>(GetScreenWidth()); }
    static float ScreenY() { return static_cast<float>(GetScreenHeight()); }
    static float HalfScreenX() { return ScreenX() / 2.0f; }
    static float HalfScreenY() { return ScreenY() / 2.0f; }

    /// Place this component in the render queue
    void EnterRenderQueue(const Component* component, const int layer) {
        render_queue_.push_back({layer, component});
    }

private:

    Engine() = default;

    float delta_accumulator_ = 0.0f;
    std::string demo_title_;
    std::string demo_controls_;

    /// Renders the scene
    void RenderScene();

    struct RenderQueueEntry {
        int layer = 0;
        const Component* component = nullptr;
    };

    std::vector<RenderQueueEntry> render_queue_ = std::vector<RenderQueueEntry>();

    /// Draw a single line on screen, with specified length, thickness, angle, center, color
    static void DrawLine(float length, float thickness, float angle, const Vector2& center, const Color& color);

    /// Draw a grid on screen
    void DrawGrid() const ;
};




#endif
