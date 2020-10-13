#pragma once

enum class TileType {
    WALL = 0,
    FLOOR = 1,
    DOOR = 2,
    MASK = 3,
    MONSTER = 4,
    CHEST = 5,
    START = 6,
    FINISH = 7,
};

class Tile {
public:
    TileType type = TileType::WALL;
    int roomNumber = 0;
};
