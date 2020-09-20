#pragma once

#include <limits>
#include <tuple>
#include <cmath>
#include "Constants.h"

class Dungeon;

struct TileCoord {
    int x;
    int y;

    static TileCoord Invalid() {
        return { std::numeric_limits<int>::min(), std::numeric_limits<int>::min() };
    }

    explicit operator bool() const {
        return x > std::numeric_limits<int>::min() && y > std::numeric_limits<int>::min();
    }

    [[ nodiscard ]]
    TileCoord Transform(const Dungeon & dungeon, Rotation rotation) const;

    [[ nodiscard ]]
    TileCoord operator + (const TileCoord & rhs) const {
        return { x + rhs.x, y + rhs.y };
    }

    [[ nodiscard ]]
    TileCoord operator - (const TileCoord & rhs) const {
        return { x - rhs.x, y - rhs.y };
    }

    bool operator < (const TileCoord & rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    [[ nodiscard ]]
    double DistancePythagorean(const TileCoord & rhs) const {
        return std::sqrt(std::pow(x - rhs.x, 2) + std::pow(y - rhs.y, 2));
    }
};
