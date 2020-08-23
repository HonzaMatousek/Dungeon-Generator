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
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            switch(tiles[row][col].type) {
                case TileType::WALL:
                    if(CountNeighbors4(col, row, TileType::WALL, true) > 3) {
                        std::cout << ' ';
                    }
                    else {
                        std::cout << '#';
                    }
                    break;
                case TileType::FLOOR:
                    std::cout << (char)('0' + tiles[row][col].roomNumber);
                    break;
            }
        }
        std::cout << std::endl;
    }
}

void Dungeon::Generate() {
    auto r = std::random_device();
    auto g = std::mt19937(r());
    auto d = std::uniform_int_distribution(0, 1);
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(col == 0 || col == width - 1 || row == 0 || row == height - 1) {
                tiles[row][col].type = TileType::WALL;
                continue;
            }
            tiles[row][col].type = (d(g)) ? TileType::FLOOR : TileType::WALL;
        }
    }
    for(int i = 0; i < 2; i++) {
        for (int col = 0; col < width; col++) {
            for (int row = 0; row < height; row++) {
                switch(tiles[row][col].type) {
                    case TileType::WALL:
                        if(CountNeighbors8(col, row, TileType::FLOOR) > 4) {
                            tiles[row][col].type = TileType::FLOOR;
                        }
                        break;
                    case TileType::FLOOR:
                        if(CountNeighbors8(col, row, TileType::WALL) > 4) {
                            tiles[row][col].type = TileType::WALL;
                        }
                        break;
                }
            }
        }
    }
    {
        int findX, findY;
        int roomCounter = 0;
        while(FindTile(0, TileType::FLOOR, findX, findY)) {
            roomCounter++;
            RoomFlood4(roomCounter, TileType::FLOOR, findX, findY);
        }
        auto roomNumberDistribution = std::uniform_int_distribution(1, roomCounter);
        auto randomDirection = std::uniform_int_distribution(0,1);
        for(int i = 0; i < 100; i++) {
            int roomNumber = roomNumberDistribution(g);
            if(!FindRandomTile(roomNumber, TileType::FLOOR, findX, findY, g)) continue;
            int diffX = randomDirection(g) ? 1 : -1;
            int diffY = 0;
            if(randomDirection(g)) {
                std::swap(diffX, diffY);
            }
            bool success = false;
            for(int x = findX, y = findY; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
                if(CountNeighbors8OfRoom(x, y, TileType::FLOOR, 0) > 2) {
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
                for(int x = findX, y = findY; x > 0 && y > 0 && x < width - 1 && y < height - 1; x += diffX, y += diffY) {
                    if(tiles[y][x].type == TileType::WALL) {
                        break;
                    }
                    if(tiles[y][x].type == TileType::FLOOR && tiles[y][x].roomNumber == 0) {
                        tiles[y][x].type = TileType::WALL;
                    }
                }
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

void Dungeon::RoomFlood4(int roomNumber, TileType type, int x, int y) {
    if(tiles[y][x].roomNumber == roomNumber || tiles[y][x].type != type) return;
    tiles[y][x].roomNumber = roomNumber;
    if(y > 0) RoomFlood4(roomNumber, type, x, y - 1);
    if(y < height - 1) RoomFlood4(roomNumber, type, x, y + 1);
    if(x > 0) RoomFlood4(roomNumber, type, x - 1, y);
    if(x < width - 1) RoomFlood4(roomNumber, type, x + 1, y);
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
            if(tiles[row][col].roomNumber == roomNumber && tiles[row][col].type == type) {
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
