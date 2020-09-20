#pragma once

#include "FurnitureStyle.h"

class SingleFurniture : public FurnitureStyle {
    TileType furnitureTileType;
public:
    SingleFurniture(TileType furnitureTileType);

    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};