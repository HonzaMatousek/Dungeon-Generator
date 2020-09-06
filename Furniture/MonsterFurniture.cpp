#include "MonsterFurniture.h"

void MonsterFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    size_t numberOfTiles = dungeon.CountRoomTiles(roomNumber, TileType::FLOOR);
    size_t monstersToPlace = std::sqrt(numberOfTiles);
    for(size_t i = 0; i < monstersToPlace; i++) {
        int findx, findy;
        if(dungeon.FindRandomTile(roomNumber, TileType::FLOOR, findx, findy, gen)) {
            dungeon.at(findx, findy).type = TileType::MONSTER;
        }
    }
}

std::unique_ptr<FurnitureStyle> MonsterFurniture::Clone() const {
    return std::make_unique<MonsterFurniture>(*this);
}
