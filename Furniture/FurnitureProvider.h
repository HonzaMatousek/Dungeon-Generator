#pragma once

#include <vector>
#include <memory>
#include "FurnitureStyle.h"

class FurnitureProvider {
    std::vector<std::unique_ptr<FurnitureStyle>> furnitureStyles;
public:
    void RegisterFurnitureStyle(std::unique_ptr<FurnitureStyle> && furnitureStyle);
    std::unique_ptr<FurnitureStyle> RandomFurnitureStyle(std::mt19937 & gen) const;
};
