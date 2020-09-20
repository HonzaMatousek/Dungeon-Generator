#include "MonsterFurniture.h"

void MonsterFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    size_t numberOfTiles = dungeon.CountRoomTiles(roomNumber, TileType::FLOOR);
    size_t monstersToPlace = std::sqrt(numberOfTiles);
    for(size_t i = 0; i < monstersToPlace; i++) {
        if(TileCoord find = dungeon.FindRandomTile(roomNumber, TileType::FLOOR, gen)) {
            dungeon.at(find).type = TileType::MONSTER;
        }
    }
    FurnitureStyle::FurnitureRoom(dungeon, roomNumber, gen);
}

std::unique_ptr<FurnitureStyle> MonsterFurniture::Clone() const {
    return std::make_unique<MonsterFurniture>(*this);
}
