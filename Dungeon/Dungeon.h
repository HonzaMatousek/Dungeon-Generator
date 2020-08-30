#pragma once

#include <vector>
#include <random>
#include "Tile.h"

class Dungeon {
protected:
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
public:
    Dungeon(int width, int height);
    virtual void Generate();
    void Print() const;

    int CountNeighbors8(int x, int y, TileType type, bool CountEdge = false) const;
    int CountNeighbors8OfRoom(int x, int y, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors4(int x, int y, TileType type, bool CountEdge = false) const;
    bool FindTile(int roomNumber, TileType type, int & x, int & y) const;
    bool FindRandomTile(int roomNumber, TileType type, int & x, int & y, std::mt19937 & gen) const;
    int RoomFlood4(int roomNumber, TileType type, int x, int y);
    void Noise(std::mt19937 & gen);
    void Blur(int floorThreshold, int wallThreshold);
};
