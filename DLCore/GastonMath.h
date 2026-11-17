#ifndef GASTONMATH_H
#define GASTONMATH_H
#include <cmath>
#include <ostream>
#include <raylib.h>

#include "GastonData.h"

inline bool operator==(const Vector2& self, const Vector2& other) {
    return self.x == other.x && self.y == other.y;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& self) {
    return os << "{" + std::to_string(self.x) + ", " + std::to_string(self.y) + "}";
}

inline Vector2 operator+(const Vector2& self, const Vector2& other) {
    return Vector2 { self.x + other.x, self.y + other.y };
}

inline Vector2& operator+=(Vector2& self, const Vector2& other) {
    self.x += other.x;
    self.y += other.y;
    return self;
}

inline Vector2& operator*=(Vector2& self, const float& other) {
    self.x *= other;
    self.y *= other;
    return self;
}

inline Vector2 operator-(const Vector2& self, const Vector2& other) {
    return Vector2 { self.x - other.x, self.y - other.y };
}

inline Vector2& operator-=(Vector2& self, const Vector2& other) {
    self.x -= other.x;
    self.y -= other.y;
    return self;
}

inline Vector2 operator*(const Vector2& self, const float& other) {
    return Vector2 { self.x * other, self.y * other };
}

inline Vector2 operator*(const float& other, const Vector2& self) {
    return Vector2 { self.x * other, self.y * other };
}

inline Vector2 operator/(const Vector2& self, const float& other) {
    return Vector2 { self.x / other, self.y / other };
}

inline Vector2 operator-(const Vector2& one) {
    return Vector2 { one.x * -1, one.y * -1 };
}

inline bool operator==(const Color& self, const Color& other) {
    return (self.r == other.r) && (self.g == other.g) && (self.b == other.b) && (self.a == other.a);
}

/// Returns the squared length of a 2D vector
inline float length_squared(const Vector2& self) {
    return self.x * self.x + self.y * self.y;
}

inline Vector2 normalize(const Vector2& self) {
    return self / std::sqrt(length_squared(self));
}

/// Returns the dot product of two 2D vectors
inline float dot_product(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

inline float cross_product(const Vector2& a, const Vector2& b) {
    return a.x * b.y - a.y * b.x;
}

/// Returns a new 2D vector representing the result of rotating the provided vector by a specific angle, in degrees
inline Vector2 rotated(const Vector2& v, const float& angle) {
    const float angleRad = angle * DEG2RAD;
    return Vector2 {
        v.x * cos(angleRad) - v.y * sin(angleRad),
        v.x * sin(angleRad) + v.y * cos(angleRad)
    };
}

/// Returns a function to rotate a vector by a specific angle (caches the angle to avoid recalculations)
inline auto get_rotator(const float& angle) {
    const float angleRad = angle * DEG2RAD;
    const float cosA = std::cos(angleRad);
    const float sinA = std::sin(angleRad);

    return [cosA, sinA] (const Vector2& v) -> Vector2 {
        return {
            v.x * cosA - v.y * sinA,
            v.x * sinA + v.y * cosA
        };
    };
}

#endif
