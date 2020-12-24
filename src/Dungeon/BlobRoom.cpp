#include <map>
#include "BlobRoom.h"

void BlobRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    doors.clear();
    while(!success) {
        map.Reset();
        std::map<TileCoord, int> numbers;
        std::uniform_int_distribution<int> distX(0, map.GetWidth() - 1);
        std::uniform_int_distribution<int> distY(0, map.GetHeight() - 1);
        std::uniform_int_distribution<int> distZ(0, map.GetWidth() + map.GetHeight());
        for(int i = 0; i < 50; i++) {
            TileCoord tileCoord{ distX(gen), distY(gen) };
            numbers[tileCoord] = distZ(gen);
        }
        map.WalkTiles([&](const TileCoord & tileCoord) {
            for(auto & [ mountainCoord, mountainHeight ] : numbers) {
                if(mountainHeight - std::sqrt(std::pow(tileCoord.x - mountainCoord.x, 2) + std::pow(tileCoord.y - mountainCoord.y, 2)) > (map.GetWidth() + map.GetHeight()) / 1.25) {
                    map.at(tileCoord).type = TileType::FLOOR;
                }
            }
        });
        int minRoomSize = (map.GetWidth() - 2) * (map.GetHeight() - 2) * minRoomRatio;
        int maxRoomSize = (map.GetWidth() - 2) * (map.GetHeight() - 2) * maxRoomRatio;
        if(!map.KeepConnectedPart(TileType::FLOOR, minRoomSize, maxRoomSize)) {
            continue;
        }
        else {
            success = true;
        }
        GenerateDoors(std::sqrt(minRoomSize), gen);
    }
}

std::unique_ptr<Room> BlobRoom::Clone() const {
    return std::make_unique<BlobRoom>(*this);
}
