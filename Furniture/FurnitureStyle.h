#pragma once

#include <memory>
#include "../Dungeon/Dungeon.h"

class FurnitureProvider;

class FurnitureStyle {
    std::shared_ptr<FurnitureProvider> subFurnitures;
public:
    FurnitureStyle();

    virtual void FurnitureRoom(Dungeon & dungeon, int roomNumber, std::mt19937 & gen) const;
    virtual std::unique_ptr<FurnitureStyle> Clone() const = 0;

    [[ nodiscard ]]
    std::shared_ptr<FurnitureProvider> getSubFurnitures() const;
};
