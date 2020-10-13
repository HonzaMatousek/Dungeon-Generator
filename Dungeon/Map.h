#pragma once

#include <vector>
#include <set>
#include <functional>
#include <random>
#include "Tile.h"
#include "../Util/TileCoord.h"

class Map {
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
public:
    Map(int width, int height);

    [[ nodiscard ]]
    int CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    //int CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    //int CountNeighbors8OfOtherRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;

    [[ nodiscard ]]
    int CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;

    [[ nodiscard ]]
    uint8_t MakeNeighborsBitMask(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;

    [[ nodiscard ]]
    TileCoord FindTile(TileType type) const;

    [[ nodiscard ]]
    TileCoord FindRandomTile(TileType type, std::mt19937 & gen) const;

    [[ nodiscard ]]
    TileCoord FindRandomTileNearEdge(TileType type, std::mt19937 & gen, int minCount = 0, int maxCount = 3) const;

    [[ nodiscard ]]
    TileCoord FindRandomTileDisconnectible(TileType type, std::mt19937 & gen, const std::set<uint8_t> & validNeighborMasks) const;

    [[ nodiscard ]]
    size_t CountTiles(TileType type) const;

    void Reset();
    void Noise(std::mt19937 & gen);
    void Blur(int floorThreshold, int wallThreshold);
    bool KeepConnectedPart(TileType type, int minTileCount, int maxTileCount);

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

    [[ nodiscard ]]
    int GetWidth() const;

    [[ nodiscard ]]
    int GetHeight() const;

    [[ nodiscard ]]
    TileCoord FindMinimumBB() const;

    [[ nodiscard ]]
    TileCoord FindMaximumBB() const;
};
