#pragma once

#include <vector>
#include <random>
#include <functional>
#include <set>
#include "Tile.h"
#include "../Util/Constants.h"
#include "../Util/TileCoord.h"
#include "../Settings/GeneratorPreset.h"
#include "Map.h"

class Room;
class DungeonRoom;

class Dungeon {
protected:
    Map map;
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::vector<bool>> occupancyBitmap;

    friend DungeonRoom;
public:
    Dungeon(int width, int height);
    void GenerateDungeon(const GeneratorPreset & generatorPreset, std::mt19937 & gen);
    void FinishDungeon(bool preserveDoors = false);

    bool PlaceRoom(std::unique_ptr<Room> && room, TileCoord position, Rotation rotation);
    void RemoveLastRoom();
    bool TryPlaceRoomRandomly(std::unique_ptr<Room> && otherRoom, std::mt19937 & gen);

    [[ nodiscard ]]
    const Map &GetMap() const;
};
