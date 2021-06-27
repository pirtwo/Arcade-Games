#ifndef RANDOM_H_INCLUDE
#define RANDOM_H_INCLUDE

#include <random>

class Random
{
protected:
    std::mt19937 _gen{std::random_device{}()};

public:
    Random() = default;

    Random(std::mt19937::result_type seed) : _gen(seed) {}

    float randFloat(float min, float max)
    {
        return std::uniform_real_distribution<float>{min, max}(_gen);
    }

    int randInt(int min, int max)
    {
        return std::uniform_int_distribution<int>{min, max}(_gen);
    }
};

#endif