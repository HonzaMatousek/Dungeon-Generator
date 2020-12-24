#pragma once

#include "Room.h"

class DungeonRoom : public Room {
public:
    explicit DungeonRoom(Dungeon & dungeon);
    void Generate(std::mt19937 & gen) override;

    [[nodiscard]]
    std::unique_ptr<Room> Clone() const override;
};
