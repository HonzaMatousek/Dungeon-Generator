#include <map>
#include "BlobRoom.h"

void BlobRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    doors.clear();
    while(!success) {
        Reset();
        std::map<TileCoord, int> numbers;
        std::uniform_int_distribution<int> distX(0,width - 1);
        std::uniform_int_distribution<int> distY(0, height - 1);
        std::uniform_int_distribution<int> distZ(0, width + height);
        for(int i = 0; i < 50; i++) {
            TileCoord tileCoord{ distX(gen), distY(gen) };
            numbers[tileCoord] = distZ(gen);
        }
        WalkTiles([&](const TileCoord & tileCoord) {
            for(auto & [ mountainCoord, mountainHeight ] : numbers) {
                if(mountainHeight - std::sqrt(std::pow(tileCoord.x - mountainCoord.x, 2) + std::pow(tileCoord.y - mountainCoord.y, 2)) > (width + height) / 1.25) {
                    at(tileCoord).type = TileType::FLOOR;
                }
            }
        });
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
            GenerateDoors(std::sqrt(bestRoomSize), gen);
        }
    }
}

std::unique_ptr<Room> BlobRoom::Clone() const {
    return std::make_unique<BlobRoom>(*this);
}
