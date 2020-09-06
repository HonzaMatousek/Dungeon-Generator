#pragma once

#include <vector>
#include <random>
#include "Tile.h"
#include "../Util/Constants.h"
#include "../Util/TileCoord.h"

class Room;

class Dungeon {
protected:
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
    std::vector<TileCoord> doors;
public:
    Dungeon(int width, int height);
    void GenerateDungeon();
    void Print() const;

    int CountNeighbors8(int x, int y, TileType type, bool CountEdge = false) const;
    int CountNeighbors8OfSameRoom(int x, int y, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors8OfOtherRoom(int x, int y, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors4(int x, int y, TileType type, bool CountEdge = false) const;
    TileCoord FindTile(int roomNumber, TileType type) const;
    TileCoord FindRandomTile(int roomNumber, TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileNearEdge(int roomNumber, TileType type, std::mt19937 & gen) const;
    size_t CountRoomTiles(int roomNumber, TileType type) const;
    int RoomFlood4(int roomNumber, TileType type, int x, int y);
    void Reset();
    void Noise(std::mt19937 & gen);
    void Blur(int floorThreshold, int wallThreshold);
    bool PlaceRoom(const Room & room, int roomNumber, int x, int y, Rotation rotation);

    [[ nodiscard ]]
    Tile & at(int x, int y);

    [[ nodiscard ]]
    const Tile & at(int x, int y) const;

    [[ nodiscard ]]
    Tile & at(const TileCoord & tileCoord);

    [[ nodiscard ]]
    const Tile & at(const TileCoord & tileCoord) const;

    [[ nodiscard ]]
    int getWidth() const;

    [[ nodiscard ]]
    int getHeight() const;
};
