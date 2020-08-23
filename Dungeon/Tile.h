#pragma once

enum class TileType {
    WALL = 0,
    FLOOR = 1
};

class Tile {
public:
    TileType type;
};
