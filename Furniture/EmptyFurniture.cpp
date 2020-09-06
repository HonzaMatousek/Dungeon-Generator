#include "EmptyFurniture.h"

void EmptyFurniture::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    // intentionally does nothing
}

std::unique_ptr<FurnitureStyle> EmptyFurniture::Clone() const {
    return std::make_unique<EmptyFurniture>(*this);
}
