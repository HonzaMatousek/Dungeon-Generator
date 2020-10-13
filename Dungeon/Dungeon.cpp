#include <iostream>
#include "Dungeon.h"
#include "Room.h"
#include "../Util/Random.h"
#include "RoomProvider.h"
#include "../Furniture/FurnitureStyle.h"
#include "../Furniture/FurnitureProvider.h"
#include <random>
#include <memory>
#include <set>

Dungeon::Dungeon(int width, int height) : map(width, height) {
    occupancyBitmap.resize(height);
    for(auto & row : occupancyBitmap) {
        row.resize(width);
        for(auto && bit : row) {
            bit = false;
        }
    }
}

void Dungeon::GenerateDungeon(const GeneratorPreset & generatorPreset, std::mt19937 & gen) {
    if(rooms.empty()) {
        if(auto & mask = generatorPreset.GetMask()) {
            mask->Generate(gen);
            mask->map.WalkTiles([&](const TileCoord & tileCoord) {
                if(mask->map.at(tileCoord).type == TileType::WALL) {
                    occupancyBitmap[tileCoord.y][tileCoord.x] = true;
                }
                else {
                    occupancyBitmap[tileCoord.y][tileCoord.x] = false;
                }
            });
        }
    }

    int generatedRoomCounter = 0;
    const int tryCountLimit = 50;
    int failureCounter = 0;
    const int failureLimit = 20;
    for(; generatedRoomCounter < generatorPreset.MaxRoomCount() && failureCounter < failureLimit;) {
        for (int tryCounter = 0; tryCounter < tryCountLimit && failureCounter < failureLimit; tryCounter++) {
            auto otherRoom = generatorPreset.RandomRoom(gen);
            otherRoom->Generate(gen);
            if (TryPlaceRoomRandomly((std::unique_ptr<Room> &&) std::move(otherRoom), gen)) {
                generatedRoomCounter += 1;
                failureCounter = 0;
                std::unique_ptr<FurnitureStyle> furnitureStyle = generatorPreset.RandomFurnitureStyle(gen);
                furnitureStyle->FurnitureRoom(*rooms.back(), gen);
                break;
            }
            if (tryCounter == tryCountLimit - 1) {
                if(generatedRoomCounter > 0) {
                    RemoveLastRoom();
                    generatedRoomCounter--;
                }
                tryCounter = 0;
                failureCounter++;
            }
        }
    }
}

/*int Dungeon::CountNeighbors8OfSameRoom(const TileCoord & tileCoord, TileType type, int roomNumber, bool CountEdge) const {
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
}*/

/**/

bool Dungeon::PlaceRoom(std::unique_ptr<Room> && room, TileCoord position, Rotation) {
    bool result = room->map.WalkTilesChecked([&] (const TileCoord & tileCoord) {
        //auto d = tileCoord.Transform(*this, rotation) + position;
        auto d = tileCoord + position;
        if(((d.y > map.GetHeight() - 2 || d.x > map.GetWidth() - 2 || d.y < 1 || d.x < 1) && room->map.at(tileCoord).type != TileType::WALL) || (room->map.at(tileCoord).type != TileType::WALL && room->map.at(tileCoord).type != TileType::DOOR && (occupancyBitmap[d.y][d.x] || occupancyBitmap[d.y+1][d.x] || occupancyBitmap[d.y-1][d.x] || occupancyBitmap[d.y][d.x+1] || occupancyBitmap[d.y][d.x-1] || occupancyBitmap[d.y+1][d.x+1] || occupancyBitmap[d.y-1][d.x+1] || occupancyBitmap[d.y+1][d.x-1] || occupancyBitmap[d.y-1][d.x-1]))) {
            // cannot place room here
            return false;
        }
        // can place room here
        return true;
    });

    if(!result) {
        return false;
    }

    room->map.WalkTiles([&] (const TileCoord & tileCoord) {
        auto d = tileCoord + position;
        if(room->map.at(tileCoord).type != TileType::WALL && room->map.at(tileCoord).type != TileType::DOOR) {
            occupancyBitmap[d.y][d.x] = true;
        }
    });

    rooms.emplace_back(std::move(room));
    rooms.back()->position = position;

    return true;
}

void Dungeon::RemoveLastRoom() {
    if(rooms.empty()) return;

    const auto & room = rooms.back();
    room->map.WalkTiles([&] (const TileCoord & tileCoord) {
        auto d = tileCoord + room->position;
        if(room->map.at(tileCoord).type != TileType::WALL && room->map.at(tileCoord).type != TileType::DOOR) {
            occupancyBitmap[d.y][d.x] = false;
        }
    });

    rooms.pop_back();
}

inline const std::set<uint8_t> directPathsNeighborMask{ /* NOLINT */
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

void Dungeon::FinishDungeon(bool preserveDoors) {
    for(const auto & room : rooms) {
        room->map.WalkTiles([&] (const TileCoord & tileCoord) {
            auto d = tileCoord + room->position;
            if(room->map.at(tileCoord).type != TileType::WALL) {
                map.at(d) = room->map.at(tileCoord);
            }
            /*if(occupancyBitmap[d.y][d.x]) {
                map.at(d).type = TileType::MASK;
            }*/
        });
    }

    if(!preserveDoors) {
        map.WalkTiles([&](const TileCoord &tileCoord) {
            if (map.at(tileCoord).type == TileType::DOOR) {
                auto mask = map.MakeNeighborsBitMask(tileCoord, TileType::WALL);
                if (directPathsNeighborMask.find(mask) != directPathsNeighborMask.end()) {
                    map.at(tileCoord).type = TileType::WALL;
                }
            }
        });
    }
}

bool Dungeon::TryPlaceRoomRandomly(std::unique_ptr<Room> && otherRoom, std::mt19937 & gen) {
    if(rooms.empty()) {
        auto minBB = otherRoom->map.FindMinimumBB();
        auto maxBB = otherRoom->map.FindMaximumBB();
        auto randomX = std::uniform_int_distribution(-minBB.x, map.GetWidth() - maxBB.x);
        auto randomY = std::uniform_int_distribution(-minBB.y, map.GetHeight() - maxBB.y);
        for(int tryCounter = 0; tryCounter < 1000; tryCounter++) {
            if (PlaceRoom(std::move(otherRoom), {randomX(gen), randomY(gen)}, Random::PickRandomRotation(gen))) {
                return true;
            }
        }
        return false;
    }
    else {
        const auto & room = Random::PickRandomElement(rooms, gen);
        const auto & door = Random::PickRandomElement(room->GetDoors(), gen);
        for (const auto &otherRoomDoor : otherRoom->GetDoors()) {
            Rotation otherRotation = Random::PickRandomRotation(gen);
            TileCoord d = otherRoomDoor; //otherRoomDoor.Transform(otherRoom, otherRotation);
            if (PlaceRoom(std::move(otherRoom), door - d + room->position, otherRotation)) {
                return true;
            }
        }
    }

    return false;
}

const Map &Dungeon::GetMap() const {
    return map;
}
