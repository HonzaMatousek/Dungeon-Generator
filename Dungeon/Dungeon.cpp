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
                    if(CountNeighbors(col, row, TileType::WALL) > 7) {
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
                        if(CountNeighbors(col, row, TileType::FLOOR) > 4) {
                            tiles[row][col].type = TileType::FLOOR;
                        }
                        break;
                    case TileType::FLOOR:
                        if(CountNeighbors(col, row, TileType::WALL) > 4) {
                            tiles[row][col].type = TileType::WALL;
                        }
                        break;
                }
            }
        }
    }
    {
        int findX, findY;
        int roomCounter = 1;
        while(FindTile(0, TileType::FLOOR, findX, findY)) {
            RoomFlood(roomCounter, TileType::FLOOR, findX, findY);
            roomCounter++;
        }
    }
}

int Dungeon::CountNeighbors(int x, int y, TileType type) const {
    int result = 0;
    if(y > 0          && x > 0)         result += (tiles[y-1][x-1].type == type);
    if(y > 0)                           result += (tiles[y-1][x  ].type == type);
    if(y > 0          && x < width - 1) result += (tiles[y-1][x+1].type == type);
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type);
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type);
    if(y < height - 1 && x > 0)         result += (tiles[y+1][x-1].type == type);
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type);
    if(y < height - 1 && x < width - 1) result += (tiles[y+1][x+1].type == type);
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

void Dungeon::RoomFlood(int roomNumber, TileType type, int x, int y) {
    if(tiles[y][x].roomNumber == roomNumber || tiles[y][x].type != type) return;
    tiles[y][x].roomNumber = roomNumber;
    if(y > 0) RoomFlood(roomNumber, type, x, y-1);
    if(y < height - 1) RoomFlood(roomNumber, type, x, y+1);
    if(x > 0) RoomFlood(roomNumber, type, x-1, y);
    if(x < width - 1) RoomFlood(roomNumber, type, x+1, y);
}
