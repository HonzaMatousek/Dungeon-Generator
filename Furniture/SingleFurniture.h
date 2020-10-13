#pragma once

#include "FurnitureStyle.h"

class SingleFurniture : public FurnitureStyle {
    TileType furnitureTileType;
public:
    explicit SingleFurniture(TileType furnitureTileType);

    void FurnitureRoom(Room & room, std::mt19937 &gen) const override;

    [[ nodiscard ]]
    std::unique_ptr<FurnitureStyle> Clone() const override;
};
