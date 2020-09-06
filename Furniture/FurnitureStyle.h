#pragma once

#include "../Dungeon/Dungeon.h"

class FurnitureStyle {
public:
    virtual void FurnitureRoom(Dungeon & dungeon, int roomNumber, std::mt19937 & gen) const = 0;
};
