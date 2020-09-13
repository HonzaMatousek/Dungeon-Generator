#pragma once

#include <vector>
#include <random>
#include <functional>
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
    void WalkTiles(const std::function<void(const TileCoord &)> & function);
    void WalkTiles(const std::function<void(const TileCoord &)> & function) const;
    bool WalkTilesChecked(const std::function<bool(const TileCoord &)> & function);
    bool WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) const;
    TileCoord WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function);
    TileCoord WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) const;
public:
    Dungeon(int width, int height);
    void GenerateDungeon(std::mt19937 & gen);
    void Print() const;

    int CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    int CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors8OfOtherRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    uint8_t MakeNeighborsBitMask(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    TileCoord FindTile(int roomNumber, TileType type) const;
    TileCoord FindRandomTile(int roomNumber, TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileNearEdge(int roomNumber, TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileDisconnectible(int roomNumber, TileType type, std::mt19937 & gen) const;
    size_t CountRoomTiles(int roomNumber, TileType type) const;
    int RoomFlood4(int roomNumber, TileType type, const TileCoord & tileCoord);
    void Reset();
    void Noise(std::mt19937 & gen);
    void Blur(int floorThreshold, int wallThreshold);
    bool PlaceRoom(const Room & room, int roomNumber, TileCoord position, Rotation rotation);

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
