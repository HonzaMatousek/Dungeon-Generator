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
#include <random>
#include <memory>
#include <set>

Dungeon::Dungeon(int width, int height) : width(width), height(height) {
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

void Dungeon::GenerateDungeon(std::mt19937 & gen) {
    BlobRoom mask(width, height);
    mask.Generate(0.6, 0.7, gen);
    WalkTiles([&](const TileCoord & tileCoord) {
        if(mask.at(tileCoord).type == TileType::WALL) {
            //at(tileCoord).type = TileType::MASK;
        }
    });
    RoomProvider roomProvider;
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(10, 10));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(20, 20));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(50, 20));
    roomProvider.RegisterRoom(std::make_unique<CaveRoom>(30, 30));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(10, 10));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(10, 5));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(15, 15));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(20, 20));
    roomProvider.RegisterRoom(std::make_unique<RectangleRoom>(30, 30));
    while(true) {
        auto initialRoom = roomProvider.RandomRoom(gen);
        initialRoom->Generate(0.3, 0.4, gen);
        auto randomx = std::uniform_int_distribution(0, width - initialRoom->width);
        auto randomy = std::uniform_int_distribution(0, height - initialRoom->height);
        if (PlaceRoom(*initialRoom, 1, { randomx(gen), randomy(gen) }, Random::PickRandomRotation(gen))) {
            break;
        }
    }
    int roomCounter = 1;
    for(int tryCounter = 0; roomCounter < 50 && tryCounter < 30000; tryCounter++) {
        auto otherRoom = roomProvider.RandomRoom(gen);
        otherRoom->Generate(0.1, 0.3, gen);
        bool success = false;
        const auto & door = Random::PickRandomElement(doors, gen);
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
    int corridorCounter = 0;
    for(int tryCounter = 0; corridorCounter < 0 && tryCounter < 10000; tryCounter++) {
        const auto & door = Random::PickRandomElement(doors, gen);
        if(CountNeighbors4(door, TileType::FLOOR) > 1) continue;
        int room1 = door.y < height - 1 ? tiles[door.y+1][door.x].roomNumber : 0;
        int room2 = door.y > 0 ? tiles[door.y-1][door.x].roomNumber : 0;
        int room3 = door.x < width - 1 ? tiles[door.y][door.x+1].roomNumber : 0;
        int room4 = door.x > 0 ? tiles[door.y][door.x-1].roomNumber : 0;
        int roomNumber = (room1 > 0) ? room1 : (room2 > 0) ? room2 : (room3 > 0) ? room3 : room4;
        auto randomDirection = std::uniform_int_distribution(0,1);
        int diffX = randomDirection(gen) ? 1 : -1;
        int diffY = 0;
        if(randomDirection(gen)) {
            std::swap(diffX, diffY);
        }
        bool success = false;
        for(int x = door.x, y = door.y; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
            if(CountNeighbors8OfSameRoom({ x, y }, TileType::FLOOR, 0) > 3) {
                break;
            }
            if(CountNeighbors8({ x, y }, TileType::FLOOR) > 4) {
                break;
            }
            if(tiles[y][x].type == TileType::FLOOR && tiles[y][x].roomNumber != roomNumber) {
                success = true;
                break;
            }
            if(tiles[y][x].type == TileType::WALL) {
                tiles[y][x].type = TileType::FLOOR;
            }
        }
        if(!success) {
            for(int x = door.x, y = door.y; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
                if(tiles[y][x].type == TileType::WALL) {
                    break;
                }
                if(tiles[y][x].type == TileType::FLOOR && tiles[y][x].roomNumber == 0) {
                    tiles[y][x].type = TileType::WALL;
                }
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
    FurnitureProvider furnitureProvider;
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<MonsterFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<ChestFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>());
    furnitureProvider.RegisterFurnitureStyle(std::make_unique<EmptyFurniture>());
    for(int i = 0; i < roomCounter; i++) {
        std::unique_ptr<FurnitureStyle> furnitureStyle = furnitureProvider.RandomFurnitureStyle(gen);
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
        for (int col = 0; col < width; col++) {
            for (int row = 0; row < height; row++) {

            }
        }
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
