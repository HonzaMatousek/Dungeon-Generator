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

    void WalkTiles(const std::function<void(const TileCoord &)> & function);
    bool WalkTilesChecked(const std::function<bool(const TileCoord &)> & function);
    TileCoord WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function);
    int CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    //int CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    //int CountNeighbors8OfOtherRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge = false) const;
    int CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    uint8_t MakeNeighborsBitMask(const TileCoord & tileCoord, TileType type, bool CountEdge = false) const;
    TileCoord FindTile(TileType type) const;
    TileCoord FindRandomTile(TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileNearEdge(TileType type, std::mt19937 & gen) const;
    TileCoord FindRandomTileDisconnectible(TileType type, std::mt19937 & gen, const std::set<uint8_t> & validNeighborMasks) const;
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

    int GetWidth() const;
    int GetHeight() const;

    TileCoord FindMinimumBB() const;
    TileCoord FindMaximumBB() const;
};
