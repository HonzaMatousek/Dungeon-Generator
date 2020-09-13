#pragma once

#include "FurnitureStyle.h"

class MazeFurniture : public FurnitureStyle {
public:
    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};
