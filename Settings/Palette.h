#pragma once

#include <memory>
#include <random>
#include "../Dungeon/RoomProvider.h"
#include "../Furniture/FurnitureProvider.h"

class Palette {
public:
    RoomProvider roomProvider;
    FurnitureProvider furnitureProvider;
};
