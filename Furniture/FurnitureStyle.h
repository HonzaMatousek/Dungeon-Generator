#pragma once

#include <memory>
#include "../Dungeon/Dungeon.h"

class FurnitureStyle {
public:
    virtual void FurnitureRoom(Dungeon & dungeon, int roomNumber, std::mt19937 & gen) const = 0;
    virtual std::unique_ptr<FurnitureStyle> Clone() const = 0;
};
