//
// Created by honza on 06.09.20.
//

#include "TileCoord.h"
#include "../Dungeon/Dungeon.h"

TileCoord TileCoord::Transform(const Dungeon &dungeon, Rotation rotation) const {
    return { x, y };
    /*switch(rotation) {
        case Rotation::D0:
            return { x, y };
        case Rotation::D90:
            return { y, dungeon.getWidth() - 1 - x };
        case Rotation::D180:
            return { dungeon.getWidth() - 1 - x, dungeon.getHeight() - 1 - y };
        case Rotation::D270:
            return { dungeon.getHeight() - 1 - y, x };
    }*/
}

