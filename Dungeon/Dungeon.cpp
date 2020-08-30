#include <iostream>
#include "Dungeon.h"
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

void Dungeon::Generate() {
    auto r = std::random_device();
    auto g = std::mt19937(r());
    Noise(g);
    for(int i = 0; i < 5; i++) {
        Blur(5, 5);
    }
    {
        int findX, findY;
        int roomCounter = 0;
        while(FindTile(0, TileType::FLOOR, findX, findY)) {
            roomCounter++;
            RoomFlood4(roomCounter, TileType::FLOOR, findX, findY);
        }
        if(roomCounter < 2) return;
        auto roomNumberDistribution = std::uniform_int_distribution(1, roomCounter);
        auto randomDirection = std::uniform_int_distribution(0,1);
        auto rotationDice = std::uniform_int_distribution(1, 20);
        int corridorCounter = 0;
        std::vector<std::vector<int>> roomConnections;
        roomConnections.resize(roomCounter + 1);
        for(auto & source : roomConnections) {
            source.resize(roomCounter + 1);
            for(auto & target : source) {
                target = 0;
            }
        }
        for(int tryCounter = 0; corridorCounter < roomCounter * 1.5; tryCounter++) {
            int roomWithFewestConnections = 1;
            std::vector<int> roomsWithFewestConnections;
            roomsWithFewestConnections.push_back(1);
            for(int i = 2; i <= roomCounter; i++) {
                if(roomConnections[i][0] < roomConnections[roomWithFewestConnections][0]) {
                    roomsWithFewestConnections.clear();
                    roomsWithFewestConnections.push_back(i);
                    roomWithFewestConnections = i;
                }
                else if(roomConnections[i][0] == roomConnections[roomWithFewestConnections][0]) {
                    roomsWithFewestConnections.push_back(i);
                }
            }
            int roomNumber =  roomsWithFewestConnections[std::uniform_int_distribution<size_t>(0, roomsWithFewestConnections.size() - 1)(g)];
            if(tryCounter > 10000) {
                if(roomConnections[roomNumber][0] >= roomCounter - 1) {
                    return;
                }
                std::cout << roomCounter << std::endl;
                std::cout << "FAIL. Trying to connect room " << roomNumber << " (" << (char)('0' + roomNumber) << "), without success." << std::endl;
                Print();
                std::cout << std::endl;
                for (int col = 0; col < width; col++) {
                    for (int row = 0; row < height; row++) {
                        tiles[row][col].roomNumber = 0;
                    }
                }
                int roomCounter = 0;
                while(FindTile(0, TileType::FLOOR, findX, findY)) {
                    roomCounter++;
                    RoomFlood4(roomCounter, TileType::FLOOR, findX, findY);
                }
                std::cout << roomCounter << " separate areas" << std::endl;
                return;
            }
            if(!FindRandomTile(roomNumber, TileType::FLOOR, findX, findY, g)) continue;
            int diffX = randomDirection(g) ? 1 : -1;
            int diffY = 0;
            if(randomDirection(g)) {
                std::swap(diffX, diffY);
            }
            bool success = false;
            std::vector<std::pair<int, int>> corridorTiles;
            for(int x = findX, y = findY; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
                if(CountNeighbors8OfRoom(x, y, TileType::FLOOR, 0) > 2) {
                    break;
                }
                if(diffY != 1 && tiles[y+1][x].type == TileType::FLOOR && tiles[y+1][x].roomNumber != roomNumber && tiles[y+1][x].roomNumber != 0 && tiles[y+1][x].roomNumber != tiles[y+diffY][x+diffY].roomNumber) {
                    break;
                }
                if(diffY != -1 && tiles[y-1][x].type == TileType::FLOOR && tiles[y-1][x].roomNumber != roomNumber && tiles[y-1][x].roomNumber != 0 && tiles[y-1][x].roomNumber != tiles[y+diffY][x+diffY].roomNumber) {
                    break;
                }
                if(diffX != 1 && tiles[y][x+1].type == TileType::FLOOR && tiles[y][x+1].roomNumber != roomNumber && tiles[y][x+1].roomNumber != 0 && tiles[y][x+1].roomNumber != tiles[y+diffY][x+diffY].roomNumber) {
                    break;
                }
                if(diffX != -1 && tiles[y][x-1].type == TileType::FLOOR && tiles[y][x-1].roomNumber != roomNumber && tiles[y][x-1].roomNumber != 0 && tiles[y][x-1].roomNumber != tiles[y+diffY][x+diffY].roomNumber) {
                    break;
                }
                if(tiles[y][x].type == TileType::FLOOR && tiles[y][x].roomNumber != roomNumber) {
                    if(tiles[y][x].roomNumber == 0) break;
                    success = true;
                    ConnectTwoRooms(roomNumber, tiles[y][x].roomNumber, roomConnections);
                    break;
                }
                if(tiles[y][x].type == TileType::WALL) {
                    corridorTiles.push_back({ x, y });
                    tiles[y][x].type = TileType::FLOOR;
                }
                if(rotationDice(g) == 1) {
                    if(randomDirection(g)) {
                        std::swap(diffX, diffY);
                        diffY *= -1;
                    }
                    else {
                        std::swap(diffX, diffY);
                        diffX *= -1;
                    }
                }
            }
            if(!success) {
                for(auto & tileCoordinates : corridorTiles) {
                    tiles[tileCoordinates.second][tileCoordinates.first].type = TileType::WALL;
                }
            }
            else {
                corridorCounter++;
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

int Dungeon::CountNeighbors8OfRoom(int x, int y, TileType type, int roomNumber, bool CountEdge) const {
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
