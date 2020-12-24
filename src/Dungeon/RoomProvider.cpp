//
// Created by honza on 06.09.20.
//

#include "RoomProvider.h"
#include "../Util/Random.h"

void RoomProvider::RegisterRoom(std::unique_ptr<Room> &&room, double weight) {
    rooms.emplace_back(std::move(room));
    weights.emplace_back(weight);
}

std::unique_ptr<Room> RoomProvider::RandomRoom(std::mt19937 &gen) const {
    return Random::PickRandomElementWeighted(rooms, weights, gen)->Clone();
}
