#pragma once

#include "Room.h"

class CaveRoom : public Room {
    using Room::Room;
public:
    void Generate(double minRoomRatio, double maxRoomRatio) override;
};
