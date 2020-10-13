#pragma once

#include "Room.h"

class CorridorRoom : public Room {
    using Room::Room;
public:
    void Generate(std::mt19937 & gen) override;

    [[ nodiscard ]]
    std::unique_ptr<Room> Clone() const override;
};
