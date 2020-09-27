#pragma once

#include <vector>
#include <random>
#include "Constants.h"

namespace Random {
    template<typename T>
    const T& PickRandomElement(const std::vector<T> & vec, std::mt19937 & gen) {
        auto d = std::uniform_int_distribution<size_t>(0, vec.size() - 1);
        return vec[d(gen)];
    }

    template<typename T>
    const T& PickRandomElementWeighted(const std::vector<T> & vec, const std::vector<double> & weights, std::mt19937 & gen) {
        std::discrete_distribution<> d(weights.begin(), weights.end());
        return vec[d(gen)];
    }

    inline Rotation PickRandomRotation(std::mt19937 & gen) {
        auto d = std::uniform_int_distribution<size_t>(0, 3);
        return Rotation(d(gen));
    }
};
