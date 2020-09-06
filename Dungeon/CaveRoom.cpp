#include "CaveRoom.h"

void CaveRoom::Generate(double minRoomRatio, double maxRoomRatio) {
    bool success = false;
    doors.clear();
    while(!success) {
        auto r = std::random_device();
        auto g = std::mt19937(r());
        Noise(g);
        for (int i = 0; i < 2; i++) {
            Blur(4, 4);
        }
        {
            int roomCounter = 0;
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
            });
            GenerateDoors(std::sqrt(bestRoomSize), g);
        }
    }
}

std::unique_ptr<Room> CaveRoom::Clone() const {
    return std::make_unique<CaveRoom>(*this);
}
