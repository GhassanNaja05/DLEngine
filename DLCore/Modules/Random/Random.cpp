#include "Random.h"
int Random::RandomInt(const int max) {
    return RandomInt(0, max);
}

int Random::RandomInt(const int min, const int max) {
    return static_cast<int>(std::floor(RandomFloat() * static_cast<float>(max - min))) + min;
}

float Random::RandomRange(const float min, const float max) {
    return RandomFloat() * (max - min) + min;
}
