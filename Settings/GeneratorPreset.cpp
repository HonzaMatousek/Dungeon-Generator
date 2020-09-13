#include "GeneratorPreset.h"

#include "../Dungeon/RoomProvider.h"
#include "../Furniture/FurnitureProvider.h"

std::unique_ptr<Room> GeneratorPreset::RandomRoom(std::mt19937 &gen) const {
    return roomProvider.RandomRoom(gen);

}

std::unique_ptr<FurnitureStyle> GeneratorPreset::RandomFurnitureStyle(std::mt19937 &gen) const {
    return furnitureProvider.RandomFurnitureStyle(gen);
}

GeneratorPreset::GeneratorPreset(const RoomProvider &roomProvider, const FurnitureProvider &furnitureProvider, int maxRoomCount) : roomProvider(roomProvider), furnitureProvider(furnitureProvider), maxRoomCount(maxRoomCount) {}

int GeneratorPreset::MaxRoomCount() const {
    return maxRoomCount;
}
