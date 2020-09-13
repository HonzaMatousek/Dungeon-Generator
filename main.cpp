#include <iostream>
#include "Dungeon/Dungeon.h"
#include "Dungeon/Room.h"
#include "Furniture/FurnitureProvider.h"
#include "Dungeon/RoomProvider.h"
#include "Settings/GeneratorPreset.h"
#include "Furniture/MonsterFurniture.h"
#include "Furniture/ChestFurniture.h"
#include "Furniture/EmptyFurniture.h"
#include "Furniture/MazeFurniture.h"
#include "Dungeon/CaveRoom.h"
#include "Dungeon/RectangleRoom.h"

int main() {
    auto r = std::random_device();
    auto g = std::mt19937(r());

    RoomProvider roomProvider;
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(10, 10, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(20, 20, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(50, 20, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(30, 30, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(10, 10, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(10, 5, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(15, 15, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(20, 20, 0.1, 0.3));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(30, 30, 0.1, 0.3));

    FurnitureProvider furnitureProvider;
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<MonsterFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<ChestFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<MazeFurniture>());

    Dungeon dungeon(220, 45, 0.5, 0.8);
    //Dungeon dungeon(350, 100);
    dungeon.GenerateDungeon(GeneratorPreset(roomProvider, furnitureProvider), g);
    dungeon.Print();
    //Room room(30, 30);
    //room.Generate(0.0,0.1);
    //room.Print();
    return 0;
}
