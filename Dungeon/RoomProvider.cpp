//
// Created by honza on 06.09.20.
//

#include "RoomProvider.h"
#include "../Util/Random.h"

void RoomProvider::RegisterRoom(std::unique_ptr<Room> &&room) {
    rooms.emplace_back(std::move(room));
}

std::unique_ptr<Room> RoomProvider::RandomRoom(std::mt19937 &gen) const {
    return Random::PickRandomElement(rooms, gen)->Clone();
}
