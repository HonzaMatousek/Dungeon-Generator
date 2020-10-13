#include "MonsterFurniture.h"
#include "../Dungeon/Room.h"

void MonsterFurniture::FurnitureRoom(Room & room, std::mt19937 &gen) const {
    size_t numberOfTiles = room.map.CountTiles(TileType::FLOOR);
    size_t monstersToPlace = std::sqrt(numberOfTiles);
    for(size_t i = 0; i < monstersToPlace; i++) {
        if(TileCoord find = room.map.FindRandomTile(TileType::FLOOR, gen)) {
            room.map.at(find).type = TileType::MONSTER;
        }
    }
    FurnitureStyle::FurnitureRoom(room, gen);
}

std::unique_ptr<FurnitureStyle> MonsterFurniture::Clone() const {
    return std::make_unique<MonsterFurniture>(*this);
}
