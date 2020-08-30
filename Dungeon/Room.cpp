#include "Room.h"

void Room::GenerateDoors(int count, std::mt19937 &gen) {
    int findX, findY;
    for (int i = 0; i < std::sqrt(count); i++) {
        if(FindRandomTile(0, TileType::WALL, findX, findY, gen)) {
            doors.push_back({ findX, findY, Rotation::D0 });
        }
    }
}
