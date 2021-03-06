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
    const int maxRoomCount;
    const std::unique_ptr<Room> & mask;
public:
    GeneratorPreset(const RoomProvider & roomProvider, const FurnitureProvider & furnitureProvider, int maxRoomCount, const std::unique_ptr<Room> & mask = nullptr);

    std::unique_ptr<Room> RandomRoom(std::mt19937 & gen) const;
    std::unique_ptr<FurnitureStyle> RandomFurnitureStyle(std::mt19937 & gen) const;

    [[ nodiscard ]]
    const std::unique_ptr<Room> & GetMask() const;

    [[ nodiscard ]]
    int MaxRoomCount() const;
};

