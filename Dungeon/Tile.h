#pragma once

enum class TileType {
    WALL = 0,
    FLOOR = 1,
    DOOR = 2,
};

class Tile {
public:
    TileType type;
    int roomNumber = 0;
};
