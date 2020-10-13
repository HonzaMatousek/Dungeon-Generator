#include "RectangleRoom.h"

void RectangleRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    while(!success) {
        map.Reset();
        int roomW = std::uniform_int_distribution(0, map.GetWidth() - 2)(gen);
        int roomH = std::uniform_int_distribution(0, map.GetHeight() - 2)(gen);
        if((roomW < 2 && roomH < 2) || roomW * roomH < (map.GetWidth() - 2) * (map.GetHeight() - 2) * minRoomRatio || roomW * roomH > (map.GetWidth() - 2) * (map.GetHeight() - 2) * maxRoomRatio) {
            continue;
        }
        else {
            success = true;
        }
        map.WalkTiles([&] (const TileCoord & tileCoord) {
            if(tileCoord.x < 1 || tileCoord.y < 1 || tileCoord.x > roomW || tileCoord.y > roomH) return;
            map.at(tileCoord).type = TileType::FLOOR;
            map.at(tileCoord).roomNumber = 1;
        });
        GenerateDoors(std::sqrt(roomW * roomH), gen);
    }
}

std::unique_ptr<Room> RectangleRoom::Clone() const {
    return std::make_unique<RectangleRoom>(*this);
}
