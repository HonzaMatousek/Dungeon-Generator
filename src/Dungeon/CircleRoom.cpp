#include "CircleRoom.h"

void CircleRoom::Generate(std::mt19937 & gen) {
    map.Reset();
    double radius = std::uniform_real_distribution(minRoomRatio, maxRoomRatio)(gen) * std::min(map.GetWidth() - 2, map.GetHeight() - 2) / 2;
    TileCoord center { 1 + (int)radius, 1 + (int)radius };
    map.WalkTiles([&](const TileCoord & tileCoord) {
        if(tileCoord.DistancePythagorean(center) < radius) {
            map.at(tileCoord).type = TileType::FLOOR;
        }
    });
    GenerateDoors((int)radius * 2, gen);
}

std::unique_ptr<Room> CircleRoom::Clone() const {
    return std::make_unique<CircleRoom>(*this);
}
