#include "ChestFurniture.h"

void ChestFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    if(TileCoord find = dungeon.FindRandomTileNearEdge(roomNumber, TileType::FLOOR, gen)) {
        dungeon.at(find).type = TileType::CHEST;
    }
    FurnitureStyle::FurnitureRoom(dungeon, roomNumber, gen);
}

std::unique_ptr<FurnitureStyle> ChestFurniture::Clone() const {
    return std::make_unique<ChestFurniture>(*this);
}
