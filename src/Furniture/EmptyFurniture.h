#pragma once

#include "FurnitureStyle.h"

class EmptyFurniture : public FurnitureStyle {
public:
    [[ nodiscard ]]
    std::unique_ptr<FurnitureStyle> Clone() const override;
};
