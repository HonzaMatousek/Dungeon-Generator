#pragma once

#include "Room.h"

class BlobRoom : public Room {
    using Room::Room;
public:
    void Generate(double minRoomRatio, double maxRoomRatio) override;

    std::unique_ptr<Room> Clone() const override;
};
