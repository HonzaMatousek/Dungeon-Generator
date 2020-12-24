#include "MazeFurniture.h"
#include "../Dungeon/Room.h"

inline const std::set<uint8_t> mazeNeighborMask { /* NOLINT */
        0b00000000,
        0b00000001, 0b00000100, 0b00010000, 0b01000000,
        // 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000,
        0b00000011, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11000000, 0b10000001,
        0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b01110000, 0b11100000, 0b11000001, 0b10000011,
        0b00001111, 0b00011110, 0b00111100, 0b01111000, 0b11110000, 0b11100001, 0b11000011, 0b10000111,
        0b00011111, 0b00111110, 0b01111100, 0b11111000, 0b11110001, 0b11100011, 0b11000111, 0b10001111,
        0b00111111, 0b01111110, 0b11111100, 0b11111001, 0b11110011, 0b11100111, 0b11001111, 0b10011111,
        0b01111111, 0b11111101, 0b11110111, 0b11011111,
        // 0b01111111, 0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111, 0b10111111,
        // 0b11111111
};

inline const std::set<uint8_t> directPathsNeighborMask { /* NOLINT */
        0b00000000,
        0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000,
        0b00000011, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11000000, 0b10000001,
        0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b01110000, 0b11100000, 0b11000001, 0b10000011,
        0b00001111, 0b00011110, 0b00111100, 0b01111000, 0b11110000, 0b11100001, 0b11000011, 0b10000111,
        0b00011111, 0b00111110, 0b01111100, 0b11111000, 0b11110001, 0b11100011, 0b11000111, 0b10001111,
        0b00111111, 0b01111110, 0b11111100, 0b11111001, 0b11110011, 0b11100111, 0b11001111, 0b10011111,
        0b01111111, 0b11111101, 0b11110111, 0b11011111,
        // 0b01111111, 0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111, 0b10111111,
        // 0b11111111
};

void MazeFurniture::FurnitureRoom(Room & room, std::mt19937 &gen) const {
    const std::set<uint8_t> * selectedMazeMask;
    switch(mazeType) {
        case MAZE:
            selectedMazeMask = &mazeNeighborMask;
            break;
        case DIRECT:
            selectedMazeMask = &directPathsNeighborMask;
            break;
    }
    while(TileCoord tileCoord = room.map.FindRandomTileDisconnectible(TileType::FLOOR, gen, *selectedMazeMask)) {
        room.map.at(tileCoord).type = TileType::WALL;
        //room.map.at(tileCoord).roomNumber = 0;
    }
    FurnitureStyle::FurnitureRoom(room, gen);
}

std::unique_ptr<FurnitureStyle> MazeFurniture::Clone() const {
    return std::make_unique<MazeFurniture>(*this);
}

MazeFurniture::MazeFurniture(MazeType mazeType) : FurnitureStyle(), mazeType(mazeType) {}

/*
 *
 *
 *
 *      @@@
 *      @.@
 *      @@@
 *
 *      @@@
 *      @..
 *      @.@
 *
 *
 *      @@.
 *      ...
 *      @@@
 *
 *
 *
 *      @@.
 *      .@@
 *      ..@
 *
 */