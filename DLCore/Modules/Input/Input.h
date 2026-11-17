#ifndef INPUT_H
#define INPUT_H
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <unordered_map>
#include "../../GastonData.h"
#include "../../GastonMath.h"


class Input {
public:
    typedef int KeyCode;

    Input() = delete;
    ~Input() = delete;

    /// Check if a key was pressed this frame
    static bool IsKeyJustPressed(KeyCode key);

    /// Check if a key was released this frame
    static bool IsKeyJustReleased(KeyCode key);

    /// Check if a key is being pressed
    static bool IsKeyPressed(KeyCode key);

    /// Check if a mouse button was pressed this frame
    static bool IsMouseButtonJustPressed(int button);

    /// Check if a mouse button was released this frame
    static bool IsMouseButtonJustReleased(int button);

    /// Check if a mouse button is being pressed
    static bool IsMouseButtonPressed(int button);

    struct Keys {

        Keys() = delete;
        ~Keys() = delete;

        static constexpr KeyCode A = KEY_A;
        static constexpr KeyCode B = KEY_B;
        static constexpr KeyCode C = KEY_C;
        static constexpr KeyCode D = KEY_D;
        static constexpr KeyCode E = KEY_E;
        static constexpr KeyCode F = KEY_F;
        static constexpr KeyCode G = KEY_G;
        static constexpr KeyCode H = KEY_H;
        static constexpr KeyCode I = KEY_I;
        static constexpr KeyCode J = KEY_J;
        static constexpr KeyCode K = KEY_K;
        static constexpr KeyCode L = KEY_L;
        static constexpr KeyCode M = KEY_M;
        static constexpr KeyCode N = KEY_N;
        static constexpr KeyCode O = KEY_O;
        static constexpr KeyCode P = KEY_P;
        static constexpr KeyCode Q = KEY_Q;
        static constexpr KeyCode R = KEY_R;
        static constexpr KeyCode S = KEY_S;
        static constexpr KeyCode T = KEY_T;
        static constexpr KeyCode U = KEY_U;
        static constexpr KeyCode V = KEY_V;
        static constexpr KeyCode W = KEY_W;
        static constexpr KeyCode X = KEY_X;
        static constexpr KeyCode Y = KEY_Y;
        static constexpr KeyCode Z = KEY_Z;
        static constexpr KeyCode ZERO = KEY_ZERO;
        static constexpr KeyCode ONE = KEY_ONE;
        static constexpr KeyCode TWO = KEY_TWO;
        static constexpr KeyCode THREE = KEY_THREE;
        static constexpr KeyCode FOUR = KEY_FOUR;
        static constexpr KeyCode FIVE = KEY_FIVE;
        static constexpr KeyCode SIX = KEY_SIX;
        static constexpr KeyCode SEVEN = KEY_SEVEN;
        static constexpr KeyCode EIGHT = KEY_EIGHT;
        static constexpr KeyCode NINE = KEY_NINE;
        static constexpr KeyCode MINUS = KEY_MINUS;
        static constexpr KeyCode EQUAL = KEY_EQUAL;
        static constexpr KeyCode ESCAPE = KEY_ESCAPE;
        static constexpr KeyCode ENTER = KEY_ENTER;
        static constexpr KeyCode BACKSPACE = KEY_BACKSPACE;
        static constexpr KeyCode TAB = KEY_TAB;
        static constexpr KeyCode SPACE = KEY_SPACE;
        static constexpr KeyCode LEFT_SHIFT = KEY_LEFT_SHIFT;
        static constexpr KeyCode RIGHT_SHIFT = KEY_RIGHT_SHIFT;
        static constexpr KeyCode LEFT_CONTROL = KEY_LEFT_CONTROL;
        static constexpr KeyCode LEFT_ALT = KEY_LEFT_ALT;
        static constexpr KeyCode RIGHT_CONTROL = KEY_RIGHT_CONTROL;
        static constexpr KeyCode RIGHT_ALT = KEY_RIGHT_ALT;
        static constexpr KeyCode UP = KEY_UP;
        static constexpr KeyCode DOWN = KEY_DOWN;
        static constexpr KeyCode RIGHT = KEY_RIGHT;
        static constexpr KeyCode LEFT = KEY_LEFT;
        static constexpr KeyCode COMMA = KEY_COMMA;
        static constexpr KeyCode PERIOD = KEY_PERIOD;
        static constexpr KeyCode SLASH = KEY_SLASH;
        static constexpr KeyCode LEFT_BRACKET = KEY_LEFT_BRACKET;
        static constexpr KeyCode RIGHT_BRACKET = KEY_RIGHT_BRACKET;
        static constexpr KeyCode BACKSLASH = KEY_BACKSLASH;
        static constexpr KeyCode DELETE = KEY_DELETE;
        static constexpr KeyCode CAPS_LOCK = KEY_CAPS_LOCK;
        static constexpr KeyCode SEMICOLON = KEY_SEMICOLON;
        static constexpr KeyCode APOSTROPHE = KEY_APOSTROPHE;
    };

    struct Mouse {

        Mouse() = delete;
        ~Mouse() = delete;

        static constexpr KeyCode LEFT_BUTTON = MOUSE_BUTTON_LEFT;
        static constexpr KeyCode RIGHT_BUTTON = MOUSE_BUTTON_RIGHT;
        static constexpr KeyCode BACK_BUTTON = MOUSE_BUTTON_SIDE;
        static constexpr KeyCode FORWARD_BUTTON = MOUSE_BUTTON_EXTRA;
    };

    struct Axes {
        Axes() = delete;
        ~Axes() = delete;

        static constexpr int n_axis = 2;
        static_assert(n_axis > 0);

        /// Get the Ith horizontal axis
        template<int I>
        static float GetHorizontal() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return horizontal_[I].value_;
        }

        /// Get the Ith vertical axis
        template<int I>
        static float GetVertical() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return vertical_[I].value_;
        }

        /// Get the Ith horizontal axis raw
        template<int I>
        static float GetHorizontalRaw() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return horizontal_[I].raw_value_;
        }

        /// Get the Ith vertical axis raw
        template<int I>
        static float GetVerticalRaw() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return vertical_[I].raw_value_;
        }

        /// Get the Ith dual axis vector
        template<int I>
        static Vector2 GetDualAxis() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return limit_length(GetHorizontal<I>(), GetVertical<I>());
        }

        /// Get the Ith dual axis vector raw
        template<int I>
        static Vector2 GetDualAxisRaw() {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            return GetHorizontalRaw<I>(), GetVerticalRaw<I>();
        }

        /// Set the horizontal input for the Ith axis
        template<int I>
        static void SetHorizontalInput(const KeyCode primary_neg, const KeyCode secondary_neg,
                                       const KeyCode primary_pos, const KeyCode secondary_pos) {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            horizontal_[I].primary_neg_ = primary_neg;
            horizontal_[I].secondary_neg_ = secondary_neg;
            horizontal_[I].primary_pos_ = primary_pos;
            horizontal_[I].secondary_pos_ = secondary_pos;
        }

        /// Set the vertical input the the Ith axis
        template<int I>
        static void SetVerticalInput(const KeyCode primary_neg, const KeyCode secondary_neg,
                                       const KeyCode primary_pos, const KeyCode secondary_pos) {
            static_assert(I >= 0 && I < n_axis, "Axis index out of bounds");
            vertical_[I].primary_neg_ = primary_neg;
            vertical_[I].secondary_neg_ = secondary_neg;
            vertical_[I].primary_pos_ = primary_pos;
            vertical_[I].secondary_pos_ = secondary_pos;
        }

    private :
        struct Axis {

            Axis()
                : primary_neg_(0), secondary_neg_(0), primary_pos_(0), secondary_pos_(0), value_(0.0f)
            {  }

            Axis(const KeyCode pn, const KeyCode sn, const KeyCode pp, const KeyCode sp, const float v)
                : primary_neg_(pn), secondary_neg_(sn), primary_pos_(pp), secondary_pos_(sp), value_(v)
            {  }

            ~Axis() = default;

            KeyCode primary_neg_ = 0;
            KeyCode secondary_neg_ = 0;
            KeyCode primary_pos_ = 0;
            KeyCode secondary_pos_ = 0;
            float value_ = .0f;
            float raw_value_ = .0f;

            /// Update the axis each frame
            void UpdateAxis(const float delta) {
                float target = .0f;
                if (IsKeyPressed(primary_neg_) || IsKeyPressed(secondary_neg_)) {
                    target -= 1.0f;
                }
                if (IsKeyPressed(primary_pos_) || IsKeyPressed(secondary_pos_)) {
                    target += 1.0f;
                }
                raw_value_ = target;

                const float t = std::clamp(delta * lerp_speed_, 0.0f, 1.0f);
                value_ = std::lerp(value_, target, t);
                if (std::abs(value_) < 0.01f) value_ = 0.0f;
            }
        };

        inline static Axis horizontal_[n_axis] = { Axis{ Keys::A, Keys::LEFT, Keys::D, Keys::RIGHT, 0.0f } };
        inline static Axis vertical_[n_axis] = { Axis{ Keys::W, Keys::UP, Keys::S, Keys::DOWN, 0.0f } };

        static constexpr float lerp_speed_ = 7.0f;

        /// Limit the length of an axis vector to 1.0
        static Vector2 limit_length(const float h, const float v) {
            const auto raw_value = Vector2{ h, v };
            if (float l_sq = length_squared(raw_value); l_sq > 1.0f) {
                l_sq = sqrt(l_sq);
                return raw_value / l_sq;
            }
            return raw_value;
        }

        /// Update the value of the axis
        static void UpdateAxes(float delta) {
            for (int i = 0; i < n_axis; ++i) {
                horizontal_[i].UpdateAxis(delta);
                vertical_[i].UpdateAxis(delta);
            }
        }

        friend class Input;
    };

private:

    /// Update the input each frame
    static void Update(float delta);

    inline static std::unordered_map<int, bool> previous_ {  };
    inline static std::unordered_map<int, bool> current_ {  };

    inline static std::unordered_map<int, bool> previous_mouse_ {  };
    inline static std::unordered_map<int, bool> current_mouse_ {  };

    friend class Engine;
};



#endif
