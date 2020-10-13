#pragma once

#include "FurnitureStyle.h"

class MonsterFurniture : public FurnitureStyle {
public:
    void FurnitureRoom(Room & room, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};
