#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
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
#include "Palette.h"
#include "../Furniture/SingleFurniture.h"
#include "../Dungeon/CircleRoom.h"
#include "RoomGroup.h"
#include "../Dungeon/DungeonRoom.h"

void Parser::Run(const std::string &fileName) {
    std::ifstream inFile(fileName);
    if(!inFile) {
        throw std::runtime_error("File not found");
    }
    std::string line;
    std::map<std::string, Palette> palettes;
    RunPalette(inFile, palettes, "");
}

void Parser::RunPalette(std::istream &input, std::map<std::string, Palette> & palettes, const std::string &paletteName) {
    std::string line;
    Palette & palette = palettes[paletteName];
    std::unique_ptr<Room> mask;
    std::unique_ptr<Dungeon> dungeon;
    std::vector<std::pair<int, std::string>> roomsToGenerate;
    std::map<std::string, RoomGroup> roomGroups;
    while(std::getline(input, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "#") {
            // comment, skip
        }
        else if(command == "room") {
            std::string roomType;
            int roomWidth, roomHeight;
            double weight, roomMinDensity, roomMaxDensity;
            lineStream >> weight >> roomType >> roomWidth >> roomHeight >> roomMinDensity >> roomMaxDensity;
            if(!lineStream) {
                throw std::runtime_error("Room command failed");
            }
            if(roomType == "cave") {
                palette.roomProvider.RegisterRoom(std::make_unique<CaveRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity), weight);
            }
            else if(roomType == "blob") {
                palette.roomProvider.RegisterRoom(std::make_unique<BlobRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity), weight);
            }
            else if(roomType == "rectangle") {
                palette.roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity), weight);
            }
            else if(roomType == "circle") {
                palette.roomProvider.RegisterRoom(std::make_unique<CircleRoom>(roomWidth, roomHeight, roomMinDensity, roomMaxDensity), weight);
            }
        }
        else if(command == "group") {
            std::string roomName;
            lineStream >> roomName;
            RunRoomGroup(input, roomGroups, roomName);
        }
        else if(command == "furniture") {
            std::string furnitureType;
            double weight;
            lineStream >> weight >> furnitureType;
            if(!lineStream) {
                throw std::runtime_error("Furniture command failed");
            }
            if(furnitureType == "monster") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<MonsterFurniture>(), weight);
            }
            else if(furnitureType == "maze") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<MazeFurniture>(MAZE), weight);
            }
            else if(furnitureType == "maze_direct") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<MazeFurniture>(DIRECT), weight);
            }
            else if(furnitureType == "chest") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<ChestFurniture>(), weight);
            }
            else if(furnitureType == "empty") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>(), weight);
            }
            else if(furnitureType == "start") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<SingleFurniture>(TileType::START), weight);
            }
            else if(furnitureType == "finish") {
                palette.furnitureProvider.RegisterFurnitureStyle(std::make_unique<SingleFurniture>(TileType::FINISH), weight);
            }
            std::string then;
            lineStream >> then;
            if(then == "then") {
                RunFurnitureThen(input, palette.furnitureProvider.LastFurnitureStyle()->getSubFurnitures());
            }
        }
        else if(!paletteName.empty() && command == "end") {
            return;
        }
        else if(paletteName.empty() && command == "mask") {
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
        else if(paletteName.empty() && command == "rooms") {
            int roomCount;
            std::string roomPalette;
            lineStream >> roomCount;
            if(!lineStream) {
                throw std::runtime_error("Rooms command failed");
            }
            lineStream >> roomPalette;
            roomsToGenerate.emplace_back(roomCount, roomPalette);
        }
        else if(paletteName.empty() && command == "palette") {
            std::string newPaletteName;
            lineStream >> newPaletteName;
            if(!lineStream) {
                throw std::runtime_error("Palette command failed");
            }
            RunPalette(input, palettes, newPaletteName);
        }
        else if(paletteName.empty() && command == "dungeon") {
            int dungeonWidth, dungeonHeight;
            lineStream >> dungeonWidth >> dungeonHeight;
            if(!lineStream) {
                throw std::runtime_error("Dungeon command failed");
            }
            dungeon = std::make_unique<Dungeon>(dungeonWidth, dungeonHeight);
            std::random_device rd;
            std::mt19937 gen(rd());
            for(auto const & [ roomCount, paletteName ] : roomsToGenerate) {
                if(palettes.find(paletteName) == palettes.end()) {
                    for(int i = 0; i < roomCount; i++) {
                        RoomGroup &roomGroup = roomGroups[paletteName];
                        for (int tryCounter = 0; tryCounter < 1000; tryCounter++) {
                            Dungeon groupDungeon(dungeonWidth, dungeonHeight);
                            for (const auto &roomElement : roomGroup.rooms) {
                                Palette &usedPalette = palettes[roomElement.roomName];
                                groupDungeon.GenerateDungeon(
                                        GeneratorPreset(usedPalette.roomProvider, usedPalette.furnitureProvider,
                                                        roomElement.roomCount, mask), gen);
                                groupDungeon.FinishDungeon();
                            }
                            if (dungeon->TryPlaceRoomRandomly(DungeonRoom(groupDungeon), gen)) break;
                        }
                    }
                }
                else {
                    Palette &usedPalette = palettes[paletteName];
                    dungeon->GenerateDungeon(GeneratorPreset(usedPalette.roomProvider, usedPalette.furnitureProvider, roomCount, mask), gen);
                }
            }
            dungeon->FinishDungeon();
        }
        else if(paletteName.empty() && command == "render") {
            std::string renderType;
            lineStream >> renderType;
            std::unique_ptr<Renderer> renderer;
            if(renderType.empty()) {
                renderer = std::make_unique<OstreamRenderer>(std::cout);
            }
            else {
                renderer = std::make_unique<ImageRenderer>(renderType);
            }
            renderer->Render(dungeon->GetMap());
        }
    }
}

void Parser::RunFurnitureThen(std::istream &input, const std::shared_ptr<FurnitureProvider> &furnitureProvider) {
    std::string line;
    while(std::getline(input, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "#") {
            // do nothing
        }
        else if(command == "furniture") {
            std::string furnitureType;
            double weight;
            lineStream >> weight >> furnitureType;
            if(!lineStream) {
                throw std::runtime_error("Furniture command failed");
            }
            if(furnitureType == "monster") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<MonsterFurniture>(), weight);
            }
            else if(furnitureType == "maze") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<MazeFurniture>(MAZE), weight);
            }
            else if(furnitureType == "maze_direct") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<MazeFurniture>(DIRECT), weight);
            }
            else if(furnitureType == "chest") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<ChestFurniture>(), weight);
            }
            else if(furnitureType == "empty") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<EmptyFurniture>(), weight);
            }
            else if(furnitureType == "start") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<SingleFurniture>(TileType::START), weight);
            }
            else if(furnitureType == "finish") {
                furnitureProvider->RegisterFurnitureStyle(std::make_unique<SingleFurniture>(TileType::FINISH), weight);
            }
            std::string then;
            lineStream >> then;
            if(then == "then") {
                RunFurnitureThen(input, furnitureProvider->LastFurnitureStyle()->getSubFurnitures());
            }
        }
        else if(command == "end") {
            return;
        }
    }
}

void Parser::RunRoomGroup(std::istream &input, std::map<std::string, RoomGroup>& roomGroups, const std::string & roomName) {
    std::string line;
    while(std::getline(input, line)) {
        std::string command;
        std::istringstream lineStream(line);
        lineStream >> command;
        if(command == "#") {
            // do nothing
        }
        else if(command == "rooms") {
            int roomCount;
            std::string roomPalette;
            lineStream >> roomCount >> roomPalette;
            roomGroups[roomName].rooms.push_back({roomCount, roomPalette });
        }
        else if(command == "end") {
            return;
        }
    }
}
