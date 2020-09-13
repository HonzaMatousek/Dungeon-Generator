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
#include "Settings/Parser.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage:" << std::endl;
        std::cout << argv[0] << " <dungeon-spec-file-name>" << std::endl;
        return 1;
    }
    try {
        Parser parser;
        parser.Run(argv[1]);
    }
    catch(const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
