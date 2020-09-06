#pragma once

#include "Constants.h"

struct TileCoord {
    int x;
    int y;

    static TileCoord Invalid() {
        return { std::numeric_limits<int>::min(), std::numeric_limits<int>::min() };
    }

    operator bool() const {
        return x > std::numeric_limits<int>::min() && y > std::numeric_limits<int>::min();
    }
};
