//
// Created by honza on 27.09.20.
//

#include "DungeonRoom.h"

DungeonRoom::DungeonRoom(Dungeon &dungeon) : Room(dungeon.getWidth(), dungeon.getHeight(), 0, 0) {
    doors = dungeon.doors;
    tiles = dungeon.tiles;
}

void DungeonRoom::Generate(std::mt19937 &gen) {}

std::unique_ptr<Room> DungeonRoom::Clone() const {
    return std::make_unique<DungeonRoom>(*this);
}
