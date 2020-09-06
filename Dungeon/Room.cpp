#include "Room.h"

void Room::GenerateDoors(int count, std::mt19937 &gen) {
    for (int i = 0; i < count; i++) {
        if(TileCoord find = FindRandomTileNearEdge(0, TileType::WALL, gen)) {
            doors.push_back(find);
        }
    }
}
