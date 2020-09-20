#include "EmptyFurniture.h"

std::unique_ptr<FurnitureStyle> EmptyFurniture::Clone() const {
    return std::make_unique<EmptyFurniture>(*this);
}
