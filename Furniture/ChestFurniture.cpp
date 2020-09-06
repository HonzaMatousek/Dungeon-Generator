#include "ChestFurniture.h"

void ChestFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    int findx, findy;
    if(dungeon.FindRandomTileNearEdge(roomNumber, TileType::FLOOR, findx, findy, gen)) {
        dungeon.at(findx, findy).type = TileType::CHEST;
    }
}

std::unique_ptr<FurnitureStyle> ChestFurniture::Clone() const {
    return std::make_unique<ChestFurniture>(*this);
}
