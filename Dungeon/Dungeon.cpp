#include <iostream>
#include "Dungeon.h"
#include "Room.h"
#include "../Util/Random.h"
#include "CaveRoom.h"
#include "RectangleRoom.h"
#include "BlobRoom.h"
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
                    if(CountNeighbors8(col, row, TileType::WALL, true) > 7) {
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
            }
        }
        std::cout << std::endl;
    }
}

void ConnectTwoRooms(int source, int target, std::vector<std::vector<int>> & roomConnections) {
    for(int i = 1; i < roomConnections.size(); i++) {
        // create connections between 2 rooms
        roomConnections[target][source] = 1;
        roomConnections[source][target] = 1;
        // mark all connected rooms as connected to the other room
        roomConnections[source][i] |= roomConnections[target][i];
        roomConnections[i][source] |= roomConnections[i][target];
        roomConnections[target][i] |= roomConnections[source][i];
        roomConnections[i][target] |= roomConnections[i][source];
    }
    for(int i = 1; i < roomConnections.size(); i++) {
        roomConnections[i][0] = 0;
        for (int j = 1; j < roomConnections.size(); j++) {
            roomConnections[i][0] += roomConnections[i][j];
        }
    }
}

void Dungeon::GenerateDungeon() {
    BlobRoom mask(width, height);
    mask.Generate(0.4, 0.7);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if(mask.tiles[row][col].type == TileType::WALL) {
                tiles[row][col].type = TileType::MASK;
            }
        }
    }
    auto r = std::random_device();
    auto g = std::mt19937(r());
    std::unique_ptr<Room> initialRoom = std::make_unique<CaveRoom>(30,30);
    initialRoom->Generate(0.3, 0.4);
    auto randomx = std::uniform_int_distribution(0, width - initialRoom->width);
    auto randomy = std::uniform_int_distribution(0, height - initialRoom->height);
    while(!PlaceRoom(*initialRoom, 1, randomx(g), randomy(g), Random::PickRandomRotation(g))) {
        initialRoom->Generate(0.3, 0.4);
    }
    int roomCounter = 1;
    for(int tryCounter = 0; roomCounter < 2000 && tryCounter < 30000; tryCounter++) {
        std::unique_ptr<Room> otherRoom;
        if(std::uniform_int_distribution(0,1)(g)) {
            otherRoom = std::make_unique<CaveRoom>(20, 20);
        }
        else {
            otherRoom = std::make_unique<RectangleRoom>(15, 15);
        }
        otherRoom->Generate(0.1, 0.3);
        bool success = false;
        const auto & door = Random::PickRandomElement(doors, g);
        /*for(const auto & door : doors)*/ {
            if(tiles[door.y][door.x].roomNumber == -1) continue;
            for(const auto & otherRoomDoor : otherRoom->doors) {
                Rotation otherRotation = Random::PickRandomRotation(g);
                int dx, dy;
                TransformCoords(otherRoomDoor.x, otherRoomDoor.y, otherRoom->width, otherRoom->height, otherRotation, dx, dy);
                if(PlaceRoom(*otherRoom, roomCounter + 1, door.x - dx, door.y - dy, otherRotation)) {
                    roomCounter++;
                    success = true;
                    break;
                }
            }
            if(success) {
                tiles[door.y][door.x].roomNumber = -1;
                //break;
            }
        }
    }
    int corridorCounter = 0;
    for(int tryCounter = 0; corridorCounter < 0 && tryCounter < 10000; tryCounter++) {
        const auto & door = Random::PickRandomElement(doors, g);
        if(CountNeighbors4(door.x, door.y, TileType::FLOOR) > 1) continue;
        int room1 = door.y < height - 1 ? tiles[door.y+1][door.x].roomNumber : 0;
        int room2 = door.y > 0 ? tiles[door.y-1][door.x].roomNumber : 0;
        int room3 = door.x < width - 1 ? tiles[door.y][door.x+1].roomNumber : 0;
        int room4 = door.x > 0 ? tiles[door.y][door.x-1].roomNumber : 0;
        int roomNumber = (room1 > 0) ? room1 : (room2 > 0) ? room2 : (room3 > 0) ? room3 : room4;
        auto randomDirection = std::uniform_int_distribution(0,1);
        int diffX = randomDirection(g) ? 1 : -1;
        int diffY = 0;
        if(randomDirection(g)) {
            std::swap(diffX, diffY);
        }
        bool success = false;
        for(int x = door.x, y = door.y; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
            if(CountNeighbors8OfSameRoom(x, y, TileType::FLOOR, 0) > 3) {
                break;
            }
            if(CountNeighbors8(x, y, TileType::FLOOR) > 4) {
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
            tiles[door.y][door.x].roomNumber = 0;
        }
        else {
            tiles[door.y][door.x].type = TileType::DOOR;
            tiles[door.y][door.x].roomNumber = 0;
        }
    }
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if(tiles[row][col].type == TileType::MASK) {
                tiles[row][col].type = TileType::WALL;
            }
        }
    }
}

int Dungeon::CountNeighbors8(int x, int y, TileType type, bool CountEdge) const {
    int result = 0;
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

int Dungeon::CountNeighbors8OfSameRoom(int x, int y, TileType type, int roomNumber, bool CountEdge) const {
    int result = 0;
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

int Dungeon::CountNeighbors8OfOtherRoom(int x, int y, TileType type, int roomNumber, bool CountEdge) const {
    int result = 0;
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

bool Dungeon::FindTile(int roomNumber, TileType type, int &x, int &y) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(tiles[row][col].roomNumber == roomNumber && tiles[row][col].type == type) {
                x = col;
                y = row;
                return true;
            }
        }
    }
    return false;
}

int Dungeon::RoomFlood4(int roomNumber, TileType type, int x, int y) {
    if(tiles[y][x].roomNumber == roomNumber || tiles[y][x].type != type) return 0;
    tiles[y][x].roomNumber = roomNumber;
    int result = 1;
    if(y > 0)          result += RoomFlood4(roomNumber, type, x, y - 1);
    if(y < height - 1) result += RoomFlood4(roomNumber, type, x, y + 1);
    if(x > 0)          result += RoomFlood4(roomNumber, type, x - 1, y);
    if(x < width - 1)  result += RoomFlood4(roomNumber, type, x + 1, y);
    return result;
}

int Dungeon::CountNeighbors4(int x, int y, TileType type, bool CountEdge) const {
    int result = 0;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type); else result += CountEdge;
    return result;
}

bool Dungeon::FindRandomTile(int roomNumber, TileType type, int &x, int &y, std::mt19937 & gen) const {
    std::vector<std::pair<int, int>> roomTiles;
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(tiles[row][col].roomNumber == roomNumber && tiles[row][col].type == type && CountNeighbors4(col, row, type, true) < 4) {
                roomTiles.push_back({col, row});
            }
        }
    }
    if(roomTiles.empty()) {
        return false;
    }
    auto d = std::uniform_int_distribution<size_t>(0, roomTiles.size() - 1);
    auto & tile = roomTiles[d(gen)];
    x = tile.first;
    y = tile.second;
    return true;
}

void Dungeon::Reset() {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            tiles[row][col].roomNumber = 0;
            tiles[row][col].type = TileType::WALL;
        }
    }
}

void Dungeon::Noise(std::mt19937 &gen) {
    auto d = std::uniform_int_distribution(0, 1);
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            tiles[row][col].roomNumber = 0;
            if(col == 0 || col == width - 1 || row == 0 || row == height - 1) {
                tiles[row][col].type = TileType::WALL;
                continue;
            }
            tiles[row][col].type = (d(gen)) ? TileType::FLOOR : TileType::WALL;
        }
    }
}

void Dungeon::Blur(int floorThreshold, int wallThreshold) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            switch(tiles[row][col].type) {
                case TileType::WALL:
                    if(CountNeighbors8(col, row, TileType::FLOOR) > floorThreshold) {
                        tiles[row][col].type = TileType::FLOOR;
                    }
                    break;
                case TileType::FLOOR:
                    if(CountNeighbors8(col, row, TileType::WALL) > wallThreshold) {
                        tiles[row][col].type = TileType::WALL;
                    }
                    break;
            }
        }
    }
}

bool Dungeon::PlaceRoom(const Room &room, int roomNumber, int x, int y, Rotation rotation) {
    bool failed = false;
    for (int col = 0; !failed && col < room.width; col++) {
        for (int row = 0; !failed && row < room.height; row++) {
            int dcol, drow;
            TransformCoords(col, row, room.width, room.height, rotation, dcol, drow);
            dcol += x;
            drow += y;
            if(room.tiles[row][col].type == TileType::FLOOR) {
                if(drow > height - 2 || dcol > width - 2 || drow < 1 || dcol < 1 || tiles[drow][dcol].type == TileType::FLOOR || CountNeighbors8OfOtherRoom(dcol, drow, TileType::FLOOR, roomNumber) || tiles[drow][dcol].type == TileType::MASK) {
                    failed = true;
                    continue;
                }
                tiles[drow][dcol].type = TileType::FLOOR;
                tiles[drow][dcol].roomNumber = roomNumber;
            }
        }
    }
    if(failed) {
        for (int col = 0; col < width; col++) {
            for (int row = 0; row < height; row++) {
                if(tiles[row][col].type == TileType::FLOOR && tiles[row][col].roomNumber == roomNumber) {
                    tiles[row][col].type = TileType::WALL;
                    tiles[row][col].roomNumber = 0;
                }
            }
        }
    }
    else {
        for(auto const & door : room.doors) {
            int dcol, drow;
            TransformCoords(door.x, door.y, room.width, room.height, rotation, dcol, drow);
            dcol += x;
            drow += y;
            doors.push_back({ dcol, drow, Rotation::D0 });
        }
    }
    return !failed;
}

void Dungeon::TransformCoords(int x, int y, int width, int height, Rotation rotation, int &outX, int &outY) {
    switch(rotation) {
        case Rotation::D0:
            outX = x;
            outY = y;
            break;
        case Rotation::D90:
            outX = y;
            outY = width - 1 - x;
            break;
        case Rotation::D180:
            outX = width - 1 - x;
            outY = height - 1 - y;
            break;
        case Rotation::D270:
            outX = height - 1 - y;
            outY = x;
            break;
    }
}
