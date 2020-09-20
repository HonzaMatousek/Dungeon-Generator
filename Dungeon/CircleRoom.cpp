#include "CircleRoom.h"

void CircleRoom::Generate(std::mt19937 & gen) {
    Reset();
    double radius = std::uniform_real_distribution(minRoomRatio, maxRoomRatio)(gen) * std::min(width - 2, height - 2) / 2;
    TileCoord center { 1 + (int)radius, 1 + (int)radius };
    WalkTiles([&](const TileCoord & tileCoord) {
        if(tileCoord.DistancePythagorean(center) < radius) {
            at(tileCoord).type = TileType::FLOOR;
            at(tileCoord).roomNumber = 1;
        }
    });
    GenerateDoors((int)radius * 2, gen);
}

std::unique_ptr<Room> CircleRoom::Clone() const {
    return std::make_unique<CircleRoom>(*this);
}
