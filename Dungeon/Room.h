#pragma once

#include <memory>
#include "Dungeon.h"
#include "Map.h"

class Room {
protected:
    double minRoomRatio;
    double maxRoomRatio;
    std::vector<TileCoord> doors;
public:
    const std::vector<TileCoord> &GetDoors() const;
    TileCoord position;

protected:
    std::vector<std::shared_ptr<Room>> rooms;
public:
    Room(int width, int height, double minRoomRatio, double maxRoomRatio);
    virtual void Generate(std::mt19937 & gen) = 0;
    virtual void GenerateDoors(int count, std::mt19937 & gen);

    [[ nodiscard ]]
    virtual std::unique_ptr<Room> Clone() const = 0;

    Map map;
};
