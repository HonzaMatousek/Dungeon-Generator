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

    int CountNeighbors8(int x, int y, TileType type, bool CountEdge = false) const;
    int CountNeighbors4(int x, int y, TileType type, bool CountEdge = false) const;
    bool FindTile(int roomNumber, TileType type, int & x, int & y) const;
    void RoomFlood4(int roomNumber, TileType type, int x, int y);
};
