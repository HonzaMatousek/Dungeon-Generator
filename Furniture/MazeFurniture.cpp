#include "MazeFurniture.h"

void MazeFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    while(TileCoord tileCoord = dungeon.FindRandomTileDisconnectible(roomNumber, TileType::FLOOR, gen)) {
        dungeon.at(tileCoord).type = TileType::WALL;
        dungeon.at(tileCoord).roomNumber = 0;
    }
}

std::unique_ptr<FurnitureStyle> MazeFurniture::Clone() const {
    return std::make_unique<MazeFurniture>(*this);
}

/*
 *
 *
 *
 *      @@@
 *      @.@
 *      @@@
 *
 *      @@@
 *      @..
 *      @.@
 *
 *
 *      @@.
 *      ...
 *      @@@
 *
 *
 *
 *      @@.
 *      .@@
 *      ..@
 *
 */