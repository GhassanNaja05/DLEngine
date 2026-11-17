#ifndef RANDOM_H
#define RANDOM_H
#include <cassert>
#include <cmath>
#include <iterator>
#include <vector>
#include <random>

class Random {
public:
    Random() = delete;
    ~Random() = delete;

    /// Get a random integer between 0 and max (exclusive)
    static int RandomInt(int max);

    /// Get a random integer between min (inclusive) and max (exclusive)
    static int RandomInt(int min, int max);

    /// Get a random float between min and max
    static float RandomRange(float min, float max);

    /// Shuffle a vector randomly
    template<typename T>
    static void Shuffle(std::vector<T>& vector) {
        for (int i = vector.size() - 1; i > 0; --i) {
            const int random = RandomInt(i + 1);

            std::swap(vector[i], vector[random]);
        }
    }

    /// Pick a random element from a container
    template<typename Container>
    static auto& PickRandom(Container& c) {
        assert(!c.empty());
        auto it = std::begin(c);
        std::advance(it, RandomInt(std::size(c)));
        return *it;
    }

    /// Pick a random element from an initializer list
    template<typename T>
    static T PickRandom(std::initializer_list<T> list) {
        assert(list.size() > 0);
        auto it = list.begin();
        std::advance(it, RandomInt(list.size()));
        return *it;
    }

private:
    /// Get a random float between 0 and 1
    static float RandomFloat() {
        thread_local std::mt19937_64 gen{std::random_device{}()};
        thread_local std::uniform_real_distribution dist(0.0f, 1.0f);
        return dist(gen);
    }
};

#endif
