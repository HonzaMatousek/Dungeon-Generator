#include "ChestFurniture.h"
#include "../Dungeon/Room.h"

void ChestFurniture::FurnitureRoom(Room & room, std::mt19937 &gen) const {
    if(TileCoord find = room.map.FindRandomTileNearEdge(TileType::FLOOR, gen)) {
        room.map.at(find).type = TileType::CHEST;
    }
    FurnitureStyle::FurnitureRoom(room, gen);
}

std::unique_ptr<FurnitureStyle> ChestFurniture::Clone() const {
    return std::make_unique<ChestFurniture>(*this);
}
