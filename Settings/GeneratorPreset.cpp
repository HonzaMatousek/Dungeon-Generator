#include "GeneratorPreset.h"

#include "../Dungeon/RoomProvider.h"
#include "../Furniture/FurnitureProvider.h"
#include "../Furniture/EmptyFurniture.h"

std::unique_ptr<Room> GeneratorPreset::RandomRoom(std::mt19937 &gen) const {
    return roomProvider.RandomRoom(gen);

}

std::unique_ptr<FurnitureStyle> GeneratorPreset::RandomFurnitureStyle(std::mt19937 &gen) const {
    if(furnitureProvider.Empty()) {
        return std::make_unique<EmptyFurniture>();
    }
    return furnitureProvider.RandomFurnitureStyle(gen);
}

GeneratorPreset::GeneratorPreset(const RoomProvider &roomProvider, const FurnitureProvider &furnitureProvider, int maxRoomCount, const std::unique_ptr<Room> & mask) : roomProvider(roomProvider), furnitureProvider(furnitureProvider), maxRoomCount(maxRoomCount), mask(mask) {}

int GeneratorPreset::MaxRoomCount() const {
    return maxRoomCount;
}

std::unique_ptr<Room> GeneratorPreset::GetMask() const {
    return mask ? mask->Clone() : nullptr;
}
