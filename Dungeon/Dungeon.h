#pragma once

#include <vector>
#include "Tile.h"

class Dungeon {
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
public:
    Dungeon(int width, int height);
    void Generate();
    void Print() const;

    int CountNeighbors(int x, int y, TileType type) const;
    bool FindTile(int roomNumber, TileType type, int & x, int & y) const;
    void RoomFlood(int roomNumber, TileType type, int x, int y);
};
