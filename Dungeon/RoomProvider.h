#pragma once

#include <vector>
#include <memory>
#include "Room.h"

class RoomProvider {
    std::vector<std::unique_ptr<Room>> rooms;
public:
    void RegisterRoom(std::unique_ptr<Room> && room);
    std::unique_ptr<Room> RandomRoom(std::mt19937 & gen);
};
