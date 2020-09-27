#include "CaveRoom.h"

void CaveRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    while(!success) {
        Reset();
        Noise(gen);
        for (int i = 0; i < 2; i++) {
            Blur(4, 4);
        }
        {
            int bestRoomSize = (width - 2) * (height - 2) * minRoomRatio;
            int maxRoomSize = (width - 2) * (height - 2) * maxRoomRatio;
            int bestRoomNumber = 0;
            while(TileCoord find = FindTile(0, TileType::FLOOR)) {
                roomCounter++;
                int roomSize = RoomFlood4(roomCounter, TileType::FLOOR, find);
                if (roomSize > bestRoomSize && roomSize < maxRoomSize) {
                    bestRoomSize = roomSize;
                    bestRoomNumber = roomCounter;
                }
            }
            if(bestRoomNumber == 0) {
                continue;
            }
            else {
                success = true;
            }
            WalkTiles([&](const TileCoord & tileCoord) {
                auto &tile = at(tileCoord);
                if (tile.roomNumber != bestRoomNumber && tile.type == TileType::FLOOR) {
                    tile.roomNumber = 0;
                    tile.type = TileType::WALL;
                }
                else if(tile.roomNumber == bestRoomNumber && tile.type == TileType::FLOOR) {
                    tile.roomNumber = 1;
                }
            });
            roomCounter = 1;
            GenerateDoors(std::sqrt(bestRoomSize), gen);
        }
    }
}

std::unique_ptr<Room> CaveRoom::Clone() const {
    return std::make_unique<CaveRoom>(*this);
}
