#pragma once

#include "Dungeon.h"

class Room : public Dungeon {
public:
    using Dungeon::Dungeon;
    virtual void Generate(double minRoomRatio, double maxRoomRatio) = 0;
};
