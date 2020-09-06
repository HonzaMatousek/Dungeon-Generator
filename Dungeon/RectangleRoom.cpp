#include "RectangleRoom.h"

void RectangleRoom::Generate(double minRoomRatio, double maxRoomRatio, std::mt19937 & gen) {
    bool success = false;
    while(!success) {
        Reset();
        int roomW = std::uniform_int_distribution(0, width - 2)(gen);
        int roomH = std::uniform_int_distribution(0, height - 2)(gen);
        if((roomW < 2 && roomH < 2) || roomW * roomH < (width - 2) * (height - 2) * minRoomRatio || roomW * roomH > (width - 2) * (height - 2) * maxRoomRatio) {
            continue;
        }
        else {
            success = true;
        }
        for (int col = 1; col <= roomW; col++) {
            for (int row = 1; row <= roomH; row++) {
                tiles[row][col].type = TileType::FLOOR;
                tiles[row][col].roomNumber = 1;
            }
        }
        GenerateDoors(std::sqrt(roomW * roomH), gen);
    }
}

std::unique_ptr<Room> RectangleRoom::Clone() const {
    return std::make_unique<RectangleRoom>(*this);
}
