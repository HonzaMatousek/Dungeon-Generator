#pragma once

#include "Room.h"

class RectangleRoom : public Room {
    using Room::Room;
public:
    void Generate(std::mt19937 & gen) override;

    std::unique_ptr<Room> Clone() const override;
};
