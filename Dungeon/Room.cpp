#include "Room.h"

void Room::GenerateDoors(int count, std::mt19937 &gen) {
    for (int i = 0; i < count; i++) {
        if(TileCoord find = map.FindRandomTileNearEdge(TileType::WALL, gen)) {
            doors.push_back(find);
            map.at(find).type = TileType::DOOR;
        }
    }
}

Room::Room(int width, int height, double minRoomRatio, double maxRoomRatio) : map(width, height), minRoomRatio(minRoomRatio), maxRoomRatio(maxRoomRatio) {}

const std::vector<TileCoord> &Room::GetDoors() const {
    return doors;
}
