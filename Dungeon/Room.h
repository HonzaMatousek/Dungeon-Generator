#pragma once

#include <memory>
#include "Dungeon.h"

class Room : public Dungeon {
public:
    using Dungeon::Dungeon;
    virtual void Generate(double minRoomRatio, double maxRoomRatio, std::mt19937 & gen) = 0;
    virtual void GenerateDoors(int count, std::mt19937 & gen);

    [[ nodiscard ]]
    virtual std::unique_ptr<Room> Clone() const = 0;
};
