#include "SingleFurniture.h"
#include "../Dungeon/Room.h"

SingleFurniture::SingleFurniture(TileType furnitureTileType) : FurnitureStyle(), furnitureTileType(furnitureTileType) {}

void SingleFurniture::FurnitureRoom(Room & room, std::mt19937 &gen) const {
    if(TileCoord find = room.map.FindRandomTileNearEdge(TileType::FLOOR, gen)) {
        room.map.at(find).type = furnitureTileType;
    }
    FurnitureStyle::FurnitureRoom(room, gen);
}

std::unique_ptr<FurnitureStyle> SingleFurniture::Clone() const {
    return std::make_unique<SingleFurniture>(*this);
}
