#include <iostream>
#include "Dungeon.h"
#include "Room.h"
#include "../Util/Random.h"
#include <random>

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
                    if(CountNeighbors4(col, row, TileType::WALL, true) > 3) {
                        std::cout << ' ';
                    }
                    else {
                        if(tiles[row][col].roomNumber == -1) {
                            std::cout << "#";
                        }
                        else {
                            std::cout << "█";
                        }
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

void Dungeon::Generate(double minRoomRatio, double maxRoomRatio) {
    std::vector<Room> rooms;
    for(int i = 0; i < 1; i++) {
        Room room(20,20);
        room.Generate(0.2,0.3);
        rooms.emplace_back(std::move(room));
    }
    auto r = std::random_device();
    auto g = std::mt19937(r());
    int roomCounter = 0;
    while(roomCounter < 10) {
        auto randomRoom = Random::PickRandomElement(rooms, g);
        auto randX = std::uniform_int_distribution(0, width - randomRoom.width);
        auto randY = std::uniform_int_distribution(0, height - randomRoom.height);
        if(PlaceRoom(randomRoom, roomCounter + 1, randX(g), randY(g), Random::PickRandomRotation(g))) {
            roomCounter++;
        }
    }
    for(auto const & door : doors) {
        tiles[door.y][door.x].roomNumber = -1;
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
            if(tiles[row][col].roomNumber == roomNumber && tiles[row][col].type == type && CountNeighbors4(col, row, type) < 4) {
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
                if(drow > height - 2 || dcol > width - 2 || tiles[drow][dcol].type == TileType::FLOOR || CountNeighbors8OfOtherRoom(dcol, drow, TileType::FLOOR, roomNumber)) {
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
