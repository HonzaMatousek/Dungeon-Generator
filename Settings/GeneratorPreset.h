#pragma once

#include <random>
#include <memory>

class RoomProvider;
class FurnitureProvider;
class Room;
class FurnitureStyle;

class GeneratorPreset {
    const RoomProvider & roomProvider;
    const FurnitureProvider & furnitureProvider;
public:
    GeneratorPreset(const RoomProvider & roomProvider, const FurnitureProvider & furnitureProvider);

    std::unique_ptr<Room> RandomRoom(std::mt19937 & gen) const;
    std::unique_ptr<FurnitureStyle> RandomFurnitureStyle(std::mt19937 & gen) const;
};

