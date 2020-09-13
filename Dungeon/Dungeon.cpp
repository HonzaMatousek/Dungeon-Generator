#include <iostream>
#include "Dungeon.h"
#include "Room.h"
#include "../Util/Random.h"
#include "CaveRoom.h"
#include "RectangleRoom.h"
#include "BlobRoom.h"
#include "RoomProvider.h"
#include "../Furniture/FurnitureStyle.h"
#include "../Furniture/MonsterFurniture.h"
#include "../Furniture/FurnitureProvider.h"
#include "../Furniture/EmptyFurniture.h"
#include "../Furniture/ChestFurniture.h"
#include "../Furniture/MazeFurniture.h"
#include <random>
#include <memory>
#include <set>

Dungeon::Dungeon(int width, int height, double minRoomRatio, double maxRoomRatio) : width(width), height(height), minRoomRatio(minRoomRatio), maxRoomRatio(maxRoomRatio) {
    tiles.resize(height);
    for(auto & row : tiles) {
        row.resize(width);
        for(auto & tile : row) {
            tile.type = TileType::WALL;
        }
    }
}

void Dungeon::Print() const {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            switch(tiles[row][col].type) {
                case TileType::WALL:
                    if(CountNeighbors8({ col, row }, TileType::WALL, true) > 7) {
                        std::cout << ' ';
                    }
                    else {
                        std::cout << "█";
                    }
                    break;
                case TileType::FLOOR:
                    //std::cout << (char)('0' + tiles[row][col].roomNumber);
                    if(tiles[row][col].roomNumber == 0) {
                        std::cout << "\033[38;5;" << (30 + (tiles[row][col].roomNumber % 200)) << "m-\033[0m";
                    }
                    else {
                        std::cout << "\033[38;5;" << (30 + (tiles[row][col].roomNumber % 200)) << "m.\033[0m";
                    }
                    break;
                case TileType::DOOR:
                    std::cout << '#';
                    break;
                case TileType::MONSTER:
                    std::cout << "☻";
                    break;
                case TileType::CHEST:
                    std::cout << "☐";
                    break;
            }
        }
        std::cout << std::endl;
    }
}

void Dungeon::GenerateDungeon(const GeneratorPreset & generatorPreset, std::mt19937 & gen) {
    if(auto mask = generatorPreset.GetMask()) {
        mask->Generate(gen);
        WalkTiles([&](const TileCoord & tileCoord) {
            if(mask->at(tileCoord).type == TileType::WALL) {
                at(tileCoord).type = TileType::MASK;
            }
        });
    }
    while(true) {
        auto initialRoom = generatorPreset.RandomRoom(gen);
        initialRoom->Generate(gen);
        auto randomx = std::uniform_int_distribution(0, width - initialRoom->width);
        auto randomy = std::uniform_int_distribution(0, height - initialRoom->height);
        if (PlaceRoom(*initialRoom, 1, { randomx(gen), randomy(gen) }, Random::PickRandomRotation(gen))) {
            break;
        }
    }
    int roomCounter = 1;
    for(int tryCounter = 0; roomCounter < generatorPreset.MaxRoomCount() && tryCounter < 50 * generatorPreset.MaxRoomCount(); tryCounter++) {
        auto otherRoom = generatorPreset.RandomRoom(gen);
        otherRoom->Generate(gen);
        bool success = false;
        const auto door = Random::PickRandomElement(doors, gen);
        /*for(const auto & door : doors)*/ {
            if(at(door).roomNumber == -1) continue;
            for(const auto & otherRoomDoor : otherRoom->doors) {
                Rotation otherRotation = Random::PickRandomRotation(gen);
                TileCoord d = otherRoomDoor.Transform(*otherRoom, otherRotation);
                if(PlaceRoom(*otherRoom, roomCounter + 1, door - d, otherRotation)) {
                    roomCounter++;
                    success = true;
                    break;
                }
            }
            if(success) {
                at(door).roomNumber = -1;
                //break;
            }
        }
    }
    for(auto const & door : doors) {
        std::set<int> neighbors;
        if(door.y < height - 1 && tiles[door.y+1][door.x].roomNumber > 0) { neighbors.insert(tiles[door.y+1][door.x].roomNumber); }
        if(door.y > 0          && tiles[door.y-1][door.x].roomNumber > 0) { neighbors.insert(tiles[door.y-1][door.x].roomNumber); }
        if(door.x < width - 1  && tiles[door.y][door.x+1].roomNumber > 0) { neighbors.insert(tiles[door.y][door.x+1].roomNumber); }
        if(door.x > 0          && tiles[door.y][door.x-1].roomNumber > 0) { neighbors.insert(tiles[door.y][door.x-1].roomNumber); }
        if(neighbors.size() <= 1) {
            at(door).roomNumber = 0;
        }
        else {
            at(door).type = TileType::DOOR;
            at(door).roomNumber = 0;
        }
    }
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == TileType::MASK) {
            at(tileCoord).type = TileType::WALL;
        }
    });
    for(int i = 0; i < roomCounter; i++) {
        std::unique_ptr<FurnitureStyle> furnitureStyle = generatorPreset.RandomFurnitureStyle(gen);
        furnitureStyle->FurnitureRoom(*this, i, gen);
    }
}

int Dungeon::CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge) const {
    int result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0          && x > 0)         result += (tiles[y-1][x-1].type == type); else result += CountEdge;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type); else result += CountEdge;
    if(y > 0          && x < width - 1) result += (tiles[y-1][x+1].type == type); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type); else result += CountEdge;
    if(y < height - 1 && x > 0)         result += (tiles[y+1][x-1].type == type); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type); else result += CountEdge;
    if(y < height - 1 && x < width - 1) result += (tiles[y+1][x+1].type == type); else result += CountEdge;
    return result;
}

int Dungeon::CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge) const {
    int result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0          && x > 0)         result += (tiles[y-1][x-1].type == type && tiles[y-1][x-1].roomNumber == roomNumber); else result += CountEdge;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type && tiles[y-1][x  ].roomNumber == roomNumber); else result += CountEdge;
    if(y > 0          && x < width - 1) result += (tiles[y-1][x+1].type == type && tiles[y-1][x+1].roomNumber == roomNumber); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type && tiles[y  ][x-1].roomNumber == roomNumber); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type && tiles[y  ][x+1].roomNumber == roomNumber); else result += CountEdge;
    if(y < height - 1 && x > 0)         result += (tiles[y+1][x-1].type == type && tiles[y+1][x-1].roomNumber == roomNumber); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type && tiles[y+1][x  ].roomNumber == roomNumber); else result += CountEdge;
    if(y < height - 1 && x < width - 1) result += (tiles[y+1][x+1].type == type && tiles[y+1][x+1].roomNumber == roomNumber); else result += CountEdge;
    return result;
}

int Dungeon::CountNeighbors8OfOtherRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge) const {
    int result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0          && x > 0)         result += (tiles[y-1][x-1].type == type && tiles[y-1][x-1].roomNumber != roomNumber); else result += CountEdge;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type && tiles[y-1][x  ].roomNumber != roomNumber); else result += CountEdge;
    if(y > 0          && x < width - 1) result += (tiles[y-1][x+1].type == type && tiles[y-1][x+1].roomNumber != roomNumber); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type && tiles[y  ][x-1].roomNumber != roomNumber); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type && tiles[y  ][x+1].roomNumber != roomNumber); else result += CountEdge;
    if(y < height - 1 && x > 0)         result += (tiles[y+1][x-1].type == type && tiles[y+1][x-1].roomNumber != roomNumber); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type && tiles[y+1][x  ].roomNumber != roomNumber); else result += CountEdge;
    if(y < height - 1 && x < width - 1) result += (tiles[y+1][x+1].type == type && tiles[y+1][x+1].roomNumber != roomNumber); else result += CountEdge;
    return result;
}

int Dungeon::RoomFlood4(int roomNumber, TileType type, const TileCoord & tileCoord) {
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(tiles[y][x].roomNumber == roomNumber || tiles[y][x].type != type) return 0;
    tiles[y][x].roomNumber = roomNumber;
    int result = 1;
    if(y > 0)          result += RoomFlood4(roomNumber, type, { x, y - 1 });
    if(y < height - 1) result += RoomFlood4(roomNumber, type, { x, y + 1 });
    if(x > 0)          result += RoomFlood4(roomNumber, type, { x - 1, y });
    if(x < width - 1)  result += RoomFlood4(roomNumber, type, { x + 1, y });
    return result;
}

int Dungeon::CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge) const {
    int result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type); else result += CountEdge;
    return result;
}

uint8_t Dungeon::MakeNeighborsBitMask(const TileCoord &tileCoord, TileType type, bool CountEdge) const {
    uint8_t result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0          && x > 0)         result |= (tiles[y-1][x-1].type == type) << 0u; else result |= CountEdge << 0u;
    if(y > 0)                           result |= (tiles[y-1][x  ].type == type) << 1u; else result |= CountEdge << 1u;
    if(y > 0          && x < width - 1) result |= (tiles[y-1][x+1].type == type) << 2u; else result |= CountEdge << 2u;
    if(                  x < width - 1) result |= (tiles[y  ][x+1].type == type) << 3u; else result |= CountEdge << 3u;
    if(y < height - 1 && x < width - 1) result |= (tiles[y+1][x+1].type == type) << 4u; else result |= CountEdge << 4u;
    if(y < height - 1)                  result |= (tiles[y+1][x  ].type == type) << 5u; else result |= CountEdge << 5u;
    if(y < height - 1 && x > 0)         result |= (tiles[y+1][x-1].type == type) << 6u; else result |= CountEdge << 6u;
    if(                  x > 0)         result |= (tiles[y  ][x-1].type == type) << 7u; else result |= CountEdge << 7u;
    return result;
}

TileCoord Dungeon::FindTile(int roomNumber, TileType type) const {
    return WalkTilesUntilValid([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber == roomNumber && at(tileCoord).type == type) {
            return tileCoord;
        }
        else {
            return TileCoord::Invalid();
        }
    });
}

TileCoord Dungeon::FindRandomTile(int roomNumber, TileType type, std::mt19937 &gen) const {
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber == roomNumber && at(tileCoord).type == type) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

TileCoord Dungeon::FindRandomTileNearEdge(int roomNumber, TileType type, std::mt19937 & gen) const {
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber == roomNumber && at(tileCoord).type == type && CountNeighbors4(tileCoord, type, true) < 4) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

TileCoord Dungeon::FindRandomTileDisconnectible(int roomNumber, TileType type, std::mt19937 & gen) const {
    static std::set<uint8_t> validNeighborMasks{
        0b00000000,
        0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000,
        0b00000011, 0b00000110, 0b00001100, 0b00011000, 0b00110000, 0b01100000, 0b11000000, 0b10000001,
        0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b01110000, 0b11100000, 0b11000001, 0b10000011,
        0b00001111, 0b00011110, 0b00111100, 0b01111000, 0b11110000, 0b11100001, 0b11000011, 0b10000111,
        0b00011111, 0b00111110, 0b01111100, 0b11111000, 0b11110001, 0b11100011, 0b11000111, 0b10001111,
        0b00111111, 0b01111110, 0b11111100, 0b11111001, 0b11110011, 0b11100111, 0b11001111, 0b10011111,
        0b01111111, 0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111, 0b10111111,
        0b11111111
    };
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber == roomNumber && at(tileCoord).type == type && validNeighborMasks.find(MakeNeighborsBitMask(tileCoord, type, true) | MakeNeighborsBitMask(tileCoord, TileType::DOOR, true)) != validNeighborMasks.end()) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

void Dungeon::Reset() {
    WalkTiles([&](const TileCoord & tileCoord) {
        at(tileCoord).roomNumber = 0;
        at(tileCoord).type = TileType::WALL;
    });
}

void Dungeon::Noise(std::mt19937 &gen) {
    auto d = std::uniform_int_distribution(0, 1);
    WalkTiles([&](const TileCoord & tileCoord) {
        at(tileCoord).roomNumber = 0;
        if(tileCoord.x == 0 || tileCoord.x == width - 1 || tileCoord.y == 0 || tileCoord.y == height - 1) {
            at(tileCoord).type = TileType::WALL;
        }
        else {
            at(tileCoord).type = TileType(d(gen));
        }
    });
}

void Dungeon::Blur(int floorThreshold, int wallThreshold) {
    WalkTiles([&](const TileCoord & tileCoord) {
        switch(at(tileCoord).type) {
            case TileType::WALL:
                if(CountNeighbors8(tileCoord, TileType::FLOOR) > floorThreshold) {
                    at(tileCoord).type = TileType::FLOOR;
                }
            case TileType::FLOOR:
                if(CountNeighbors8(tileCoord, TileType::WALL) > wallThreshold) {
                    at(tileCoord).type = TileType::WALL;
                }
                break;
            default:
                break;
        }
    });
}

bool Dungeon::PlaceRoom(const Room &room, int roomNumber, TileCoord position, Rotation rotation) {
    bool failed = !room.WalkTilesChecked([&](const TileCoord & tileCoord) {
        auto d = tileCoord.Transform(room, rotation) + position;
        if(room.at(tileCoord).type == TileType::FLOOR) {
            if(d.y > height - 2 || d.x > width - 2 || d.y < 1 || d.x < 1 || at(d).type == TileType::FLOOR || CountNeighbors8OfOtherRoom(d, TileType::FLOOR, roomNumber) || at(d).type == TileType::MASK) {
                return false;
            }
            at(d).type = TileType::FLOOR;
            at(d).roomNumber = roomNumber;
        }
        return true;
    });
    if(failed) {
        WalkTiles([&](const TileCoord & tileCoord) {
            if(at(tileCoord).type == TileType::FLOOR && at(tileCoord).roomNumber == roomNumber) {
                at(tileCoord).type = TileType::WALL;
                at(tileCoord).roomNumber = 0;
            }
        });
    }
    else {
        for(auto const & door : room.doors) {
            doors.push_back(door.Transform(room, rotation) + position);
        }
    }
    return !failed;
}

size_t Dungeon::CountRoomTiles(int roomNumber, TileType type) const {
    size_t result = 0;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber == roomNumber && at(tileCoord).type == type) {
            result++;
        }
    });
    return result;
}

Tile &Dungeon::at(int x, int y) {
    return tiles[y][x];
}

const Tile &Dungeon::at(int x, int y) const {
    return tiles[y][x];
}

Tile &Dungeon::at(const TileCoord &tileCoord) {
    return at(tileCoord.x, tileCoord.y);
}

const Tile &Dungeon::at(const TileCoord &tileCoord) const {
    return at(tileCoord.x, tileCoord.y);
}

int Dungeon::getWidth() const {
    return width;
}

int Dungeon::getHeight() const {
    return height;
}

void Dungeon::WalkTiles(const std::function<void(const TileCoord &)> & function) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            function({ col, row });
        }
    }
}

void Dungeon::WalkTiles(const std::function<void(const TileCoord &)> & function) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            function({ col, row });
        }
    }
}

bool Dungeon::WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(!function({ col, row })) return false;
        }
    }
    return true;
}

bool Dungeon::WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(!function({ col, row })) return false;
        }
    }
    return true;
}

TileCoord Dungeon::WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(auto tileCoord = function({ col, row })) {
                return tileCoord;
            }
        }
    }
    return TileCoord::Invalid();
}

TileCoord Dungeon::WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(auto tileCoord = function({ col, row })) {
                return tileCoord;
            }
        }
    }
    return TileCoord::Invalid();
}
