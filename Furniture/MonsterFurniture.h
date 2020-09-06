#pragma once

#include "FurnitureStyle.h"

class MonsterFurniture : public FurnitureStyle {
public:
    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;
};
