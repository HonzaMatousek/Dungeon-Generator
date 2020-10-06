#pragma once

#include <vector>
#include <random>
#include <functional>
#include <set>
#include "Tile.h"
#include "../Util/Constants.h"
#include "../Util/TileCoord.h"
#include "../Settings/GeneratorPreset.h"

class Room;
class DungeonRoom;

class Dungeon {
protected:
    int width;
    int height;
    double minRoomRatio;
    double maxRoomRatio;
    std::vector<std::vector<Tile>> tiles;
    std::vector<TileCoord> doors;
    std::vector<int> roomDoorCounts;
    void WalkTiles(const std::function<void(const TileCoord &)> & function);
    bool WalkTilesChecked(const std::function<bool(const TileCoord &)> & function);
    TileCoord WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function);
    int roomCounter = 0;

    friend DungeonRoom;
public:
    Dungeon(int width, int height, double minRoomRatio, double maxRoomRatio);
    void GenerateDungeon(const GeneratorPreset & generatorPreset, std::mt19937 & gen);
    void FinishDungeon();

    int CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    int CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors8OfOtherRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    uint8_t MakeNeighborsBitMask(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    TileCoord FindTile(int roomNumber, TileType type) const;
    TileCoord FindRandomTile(int roomNumber, TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileNearEdge(int roomNumber, TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileDisconnectible(int roomNumber, TileType type, std::mt19937 & gen, const std::set<uint8_t> & validNeighborMasks) const;
    size_t CountRoomTiles(int roomNumber, TileType type) const;
    int RoomFlood4(int roomNumber, TileType type, const TileCoord & tileCoord);
    void Reset();
    void Noise(std::mt19937 & gen);
    void Blur(int floorThreshold, int wallThreshold);
    bool PlaceRoom(const Room & room, int roomNumber, TileCoord position, Rotation rotation);
    void RemoveLastRoom();
    bool TryPlaceRoomRandomly(const Room & otherRoom, std::mt19937 & gen);
    TileCoord FindMinimumBB() const;
    TileCoord FindMaximumBB() const;

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

    void WalkTiles(const std::function<void(const TileCoord &)> & function) const;
    bool WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) const;
    TileCoord WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) const;
};
