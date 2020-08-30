#pragma once

#include "Dungeon.h"

class Room : public Dungeon {
public:
    using Dungeon::Dungeon;
    void Generate(double minRoomRatio = 0, double maxRoomRatio = 1) override;
};
