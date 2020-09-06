#pragma once

#include "FurnitureStyle.h"

class ChestFurniture : public FurnitureStyle {
public:
    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};
