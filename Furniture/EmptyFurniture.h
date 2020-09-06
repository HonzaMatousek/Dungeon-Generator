#pragma once

#include "FurnitureStyle.h"

class EmptyFurniture : public FurnitureStyle {
public:
    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};
