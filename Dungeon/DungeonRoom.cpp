//
// Created by honza on 27.09.20.
//

#include "DungeonRoom.h"

DungeonRoom::DungeonRoom(Dungeon &dungeon) : Room(dungeon.map.GetWidth(), dungeon.map.GetHeight(), 0, 0) {
    /*doors = dungeon.doors;
    roomDoorCounts = dungeon.roomDoorCounts;
    tiles = dungeon.tiles;
    roomCounter = dungeon.roomCounter;*/
}

void DungeonRoom::Generate(std::mt19937 &gen) {}

std::unique_ptr<Room> DungeonRoom::Clone() const {
    return std::make_unique<DungeonRoom>(*this);
}
