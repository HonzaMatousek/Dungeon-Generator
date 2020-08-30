#pragma once

#include <vector>
#include <random>

namespace Random {
    template<typename T>
    T& PickRandomElement(std::vector<T> & vec, std::mt19937 & gen) {
        auto d = std::uniform_int_distribution<size_t>(0, vec.size() - 1);
        return vec[d(gen)];
    }
};
