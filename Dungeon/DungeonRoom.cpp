#include "DungeonRoom.h"

DungeonRoom::DungeonRoom(Dungeon &dungeon) : Room(dungeon.map.GetWidth(), dungeon.map.GetHeight(), 0, 0) {
    for(auto const & room : dungeon.rooms) {
        for(auto const & door : room->GetDoors()) {
            doors.push_back(door + room->position);
        }
    }
    map = dungeon.map;
}

void DungeonRoom::Generate(std::mt19937 &gen) {}

std::unique_ptr<Room> DungeonRoom::Clone() const {
    return std::make_unique<DungeonRoom>(*this);
}
