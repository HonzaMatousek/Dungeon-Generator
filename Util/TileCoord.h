#pragma once

#include <limits>
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
};
