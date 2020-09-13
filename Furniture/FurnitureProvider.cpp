#include "FurnitureProvider.h"
#include "../Util/Random.h"

void FurnitureProvider::RegisterFurnitureStyle(std::unique_ptr<FurnitureStyle> &&furnitureStyle) {
    furnitureStyles.emplace_back(std::move(furnitureStyle));
}

std::unique_ptr<FurnitureStyle> FurnitureProvider::RandomFurnitureStyle(std::mt19937 &gen) const {
    return Random::PickRandomElement(furnitureStyles, gen)->Clone();
}
