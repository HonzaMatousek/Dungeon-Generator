#include <fstream>
#include <sstream>
#include <iostream>
#include "Parser.h"
#include "../Dungeon/RoomProvider.h"
#include "../Furniture/FurnitureProvider.h"
#include "../Dungeon/CaveRoom.h"
#include "../Dungeon/BlobRoom.h"
#include "../Dungeon/RectangleRoom.h"
#include "../Furniture/MonsterFurniture.h"
#include "../Furniture/MazeFurniture.h"
#include "../Furniture/ChestFurniture.h"
#include "../Furniture/EmptyFurniture.h"
#include "../Output/OstreamRenderer.h"
#include "../Output/ImageRenderer.h"

void Parser::Run(const std::string &fileName) {
    std::ifstream inFile(fileName);
    if(!inFile) {
        throw std::runtime_error("File not found");
    }
    std::string line;
    int maxRooms = 10;
    RoomProvider roomProvider;
    FurnitureProvider furnitureProvider;
    std::unique_ptr<Room> mask;
    std::unique_ptr<Dungeon> dungeon;
    while(std::getline(inFile, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "#") {
            // comment, skip
        }
        else if(command == "room") {
            std::string roomType;
            int roomWidth, roomHeight;
            double roomMinDensity, roomMaxDensity;
            lineStream >> roomType >> roomWidth >> roomHeight >> roomMinDensity >> roomMaxDensity;
            if(!lineStream) {
                throw std::runtime_error("Room command failed");
            }
            if(roomType == "cave") {
                roomProvider.RegisterRoom(std::make_unique<CaveRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity));
            }
            else if(roomType == "blob") {
                roomProvider.RegisterRoom(std::make_unique<BlobRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity));
            }
            else if(roomType == "rectangle") {
                roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity));
            }
        }
        else if(command == "furniture") {
            std::string furnitureType;
            lineStream >> furnitureType;
            if(!lineStream) {
                throw std::runtime_error("Furniture command failed");
            }
            if(furnitureType == "monster") {
                furnitureProvider.RegisterFurnitureStyle(std::make_unique<MonsterFurniture>());
            }
            else if(furnitureType == "maze") {
                furnitureProvider.RegisterFurnitureStyle(std::make_unique<MazeFurniture>());
            }
            else if(furnitureType == "chest") {
                furnitureProvider.RegisterFurnitureStyle(std::make_unique<ChestFurniture>());
            }
            else if(furnitureType == "empty") {
                furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>());
            }
        }
        else if(command == "mask") {
            std::string roomType;
            int roomWidth, roomHeight;
            double roomMinDensity, roomMaxDensity;
            lineStream >> roomType >> roomWidth >> roomHeight >> roomMinDensity >> roomMaxDensity;
            if(!lineStream) {
                throw std::runtime_error("Mask command failed");
            }
            if(roomType == "cave") {
                mask = std::make_unique<CaveRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity);
            }
            else if(roomType == "blob") {
                mask = std::make_unique<BlobRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity);
            }
            else if(roomType == "rectangle") {
                mask = std::make_unique<RectangleRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity);
            }
        }
        else if(command == "rooms") {
            lineStream >> maxRooms;
            if(!lineStream) {
                throw std::runtime_error("Rooms command failed");
            }
        }
        else if(command == "dungeon") {
            int dungeonWidth, dungeonHeight;
            lineStream >> dungeonWidth >> dungeonHeight;
            if(!lineStream) {
                throw std::runtime_error("Dungeon command failed");
            }
            dungeon = std::make_unique<Dungeon>(dungeonWidth, dungeonHeight, 0, 0);
            std::random_device rd;
            std::mt19937 gen(rd());
            dungeon->GenerateDungeon(GeneratorPreset(roomProvider, furnitureProvider, maxRooms, mask), gen);
        }
        else if(command == "render") {
            std::string renderType;
            lineStream >> renderType;
            std::unique_ptr<Renderer> renderer;
            if(renderType == "") {
                renderer = std::make_unique<OstreamRenderer>(std::cout);
            }
            else {
                renderer = std::make_unique<ImageRenderer>(renderType);
            }
            renderer->Render(*dungeon);
        }
    }
}
