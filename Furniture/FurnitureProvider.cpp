#include "FurnitureProvider.h"
#include "../Util/Random.h"

void FurnitureProvider::RegisterFurnitureStyle(std::unique_ptr<FurnitureStyle> &&furnitureStyle, double weight) {
    furnitureStyles.emplace_back(std::move(furnitureStyle));
    weights.emplace_back(weight);
}

std::unique_ptr<FurnitureStyle> FurnitureProvider::RandomFurnitureStyle(std::mt19937 &gen) const {
    return Random::PickRandomElementWeighted(furnitureStyles, weights, gen)->Clone();
}
