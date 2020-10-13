#include "CorridorRoom.h"
#include "../Util/Random.h"

void CorridorRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    while(!success) {
        map.Reset();

        int coin = std::uniform_int_distribution(0, 1)(gen);
        int roomW = std::uniform_int_distribution(0, map.GetWidth() - 2)(gen);
        int roomH = std::uniform_int_distribution(0, map.GetHeight() - 2)(gen);
        if(coin) {
            // vertical
            roomW = 1;
            if((roomH < 2) || roomH < (map.GetHeight() - 2) * minRoomRatio || roomH > (map.GetHeight() - 2) * maxRoomRatio) {
                continue;
            }
            else {
                success = true;
            }
        }
        else {
            // horizontal
            roomH = 1;
            if((roomW < 2) || roomW < (map.GetWidth() - 2) * minRoomRatio || roomW > (map.GetWidth() - 2) * maxRoomRatio) {
                continue;
            }
            else {
                success = true;
            }
        }
        map.WalkTiles([&] (const TileCoord & tileCoord) {
            if(tileCoord.x < 1 || tileCoord.y < 1 || tileCoord.x > roomW || tileCoord.y > roomH) return;
            map.at(tileCoord).type = TileType::FLOOR;
            map.at(tileCoord).roomNumber = 1;
        });
        GenerateDoors(std::sqrt(roomW * roomH), gen);
    }
}

std::unique_ptr<Room> CorridorRoom::Clone() const {
    return std::make_unique<CorridorRoom>(*this);
}
