#pragma once

#include <vector>
#include <memory>
#include "FurnitureStyle.h"

class FurnitureProvider {
    std::vector<std::unique_ptr<FurnitureStyle>> furnitureStyles;
    std::vector<double> weights;
public:
    void RegisterFurnitureStyle(std::unique_ptr<FurnitureStyle> && furnitureStyle, double weight = 1.0);
    std::unique_ptr<FurnitureStyle> RandomFurnitureStyle(std::mt19937 & gen) const;
    std::unique_ptr<FurnitureStyle> LastFurnitureStyle() const;
    bool Empty() const;
};
