#pragma once

#include <vector>
#include <memory>
#include "Room.h"

class RoomProvider {
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<double> weights;
public:
    void RegisterRoom(std::unique_ptr<Room> && room, double weight = 1.0);
    std::unique_ptr<Room> RandomRoom(std::mt19937 & gen) const;
};
