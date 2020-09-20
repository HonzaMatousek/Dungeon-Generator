#pragma once

#include "FurnitureStyle.h"

class EmptyFurniture : public FurnitureStyle {
public:
    std::unique_ptr<FurnitureStyle> Clone() const override;
};
