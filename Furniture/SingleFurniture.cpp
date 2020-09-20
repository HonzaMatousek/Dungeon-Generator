#include "SingleFurniture.h"

SingleFurniture::SingleFurniture(TileType furnitureTileType) : FurnitureStyle(), furnitureTileType(furnitureTileType) {}

void SingleFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    if(TileCoord find = dungeon.FindRandomTileNearEdge(roomNumber, TileType::FLOOR, gen)) {
        dungeon.at(find).type = furnitureTileType;
    }
    FurnitureStyle::FurnitureRoom(dungeon, roomNumber, gen);
}

std::unique_ptr<FurnitureStyle> SingleFurniture::Clone() const {
    return std::make_unique<SingleFurniture>(*this);
}
