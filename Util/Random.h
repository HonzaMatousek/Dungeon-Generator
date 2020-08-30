#pragma once

#include <vector>
#include <random>
#include "Constants.h"

namespace Random {
    template<typename T>
    T& PickRandomElement(std::vector<T> & vec, std::mt19937 & gen) {
        auto d = std::uniform_int_distribution<size_t>(0, vec.size() - 1);
        return vec[d(gen)];
    }

    inline Rotation PickRandomRotation(std::mt19937 & gen) {
        auto d = std::uniform_int_distribution<size_t>(0, 4);
        return Rotation(d(gen));
    }
};
