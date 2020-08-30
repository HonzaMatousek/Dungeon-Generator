#pragma once

#include "Dungeon.h"

class Room : public Dungeon {
public:
    using Dungeon::Dungeon;
    virtual void Generate(double minRoomRatio, double maxRoomRatio) = 0;
    virtual void GenerateDoors(int count, std::mt19937 & gen);
};
