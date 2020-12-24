#include "CaveRoom.h"

void CaveRoom::Generate(std::mt19937 & gen) {
    bool success = false;
    while(!success) {
        map.Reset();
        map.Noise(gen);
        for (int i = 0; i < 2; i++) {
            map.Blur(4, 4);
        }
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

std::unique_ptr<Room> CaveRoom::Clone() const {
    return std::make_unique<CaveRoom>(*this);
}
