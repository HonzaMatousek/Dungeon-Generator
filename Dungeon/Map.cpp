#include <set>
#include "Map.h"
#include "../Util/Random.h"

Map::Map(int width, int height) : width(width), height(height) {
    tiles.resize(height);
    for(auto & row : tiles) {
        row.resize(width);
        for(auto & tile : row) {
            tile.type = TileType::WALL;
        }
    }
}

Tile &Map::at(int x, int y) {
    return tiles[y][x];
}

const Tile &Map::at(int x, int y) const {
    return tiles[y][x];
}

Tile &Map::at(const TileCoord &tileCoord) {
    return at(tileCoord.x, tileCoord.y);
}

const Tile &Map::at(const TileCoord &tileCoord) const {
    return at(tileCoord.x, tileCoord.y);
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

void Map::WalkTiles(const std::function<void(const TileCoord &)> & function) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            function({ col, row });
        }
    }
}

void Map::WalkTiles(const std::function<void(const TileCoord &)> & function) const {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            function({ col, row });
        }
    }
}

bool Map::WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(!function({ col, row })) return false;
        }
    }
    return true;
}

bool Map::WalkTilesChecked(const std::function<bool(const TileCoord &)> & function) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(!function({ col, row })) return false;
        }
    }
    return true;
}

TileCoord Map::WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(auto tileCoord = function({ col, row })) {
                return tileCoord;
            }
        }
    }
    return TileCoord::Invalid();
}

TileCoord Map::WalkTilesUntilValid(const std::function<TileCoord(const TileCoord &)> & function) const {
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            if(auto tileCoord = function({ col, row })) {
                return tileCoord;
            }
        }
    }
    return TileCoord::Invalid();
}

void Map::Reset() {
    WalkTiles([&](const TileCoord & tileCoord) {
        at(tileCoord).roomNumber = 0;
        at(tileCoord).type = TileType::WALL;
    });
}

void Map::Noise(std::mt19937 &gen) {
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

void Map::Blur(int floorThreshold, int wallThreshold) {
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

int Map::CountNeighbors4(const TileCoord & tileCoord, TileType type, bool CountEdge) const {
    int result = 0;
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(y > 0)                           result += (tiles[y-1][x  ].type == type); else result += CountEdge;
    if(                  x > 0)         result += (tiles[y  ][x-1].type == type); else result += CountEdge;
    if(                  x < width - 1) result += (tiles[y  ][x+1].type == type); else result += CountEdge;
    if(y < height - 1)                  result += (tiles[y+1][x  ].type == type); else result += CountEdge;
    return result;
}

int Map::CountNeighbors8(const TileCoord & tileCoord, TileType type, bool CountEdge) const {
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

uint8_t Map::MakeNeighborsBitMask(const TileCoord &tileCoord, TileType type, bool CountEdge) const {
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

TileCoord Map::FindTile(TileType type) const {
    return WalkTilesUntilValid([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type) {
            return tileCoord;
        }
        else {
            return TileCoord::Invalid();
        }
    });
}

TileCoord Map::FindRandomTile(TileType type, std::mt19937 &gen) const {
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

TileCoord Map::FindRandomTileNearEdge(TileType type, std::mt19937 & gen) const {
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type && CountNeighbors4(tileCoord, type, true) < 4) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

TileCoord Map::FindRandomTileDisconnectible(TileType type, std::mt19937 & gen, const std::set<uint8_t> & validNeighborMasks) const {
    std::vector<TileCoord> roomTiles;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type && validNeighborMasks.find(MakeNeighborsBitMask(tileCoord, type, true) | MakeNeighborsBitMask(tileCoord, TileType::DOOR, true)) != validNeighborMasks.end()) {
            roomTiles.push_back(tileCoord);
        }
    });
    if(roomTiles.empty()) {
        return TileCoord::Invalid();
    }
    return Random::PickRandomElement(roomTiles, gen);
}

size_t Map::CountTiles(TileType type) const {
    size_t result = 0;
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type) {
            result++;
        }
    });
    return result;
}

int Map::GetWidth() const {
    return width;
}

int Map::GetHeight() const {
    return height;
}

inline int RoomFlood4(int roomNumber, TileType type, const TileCoord & tileCoord, Map & map) {
    int x = tileCoord.x;
    int y = tileCoord.y;
    if(map.at(tileCoord).roomNumber == roomNumber || map.at(tileCoord).type != type) return 0;
    map.at(tileCoord).roomNumber = roomNumber;
    int result = 1;
    if(y > 0)                   result += RoomFlood4(roomNumber, type, { x, y - 1 }, map);
    if(y < map.GetHeight() - 1) result += RoomFlood4(roomNumber, type, { x, y + 1 }, map);
    if(x > 0)                   result += RoomFlood4(roomNumber, type, { x - 1, y }, map);
    if(x < map.GetWidth() - 1)  result += RoomFlood4(roomNumber, type, { x + 1, y }, map);
    return result;
}

inline TileCoord FindAvailableTile(TileType type, const Map & map) {
    return map.WalkTilesUntilValid([&](const TileCoord & tileCoord) {
        if(map.at(tileCoord).type == type && map.at(tileCoord).roomNumber == 0) {
            return tileCoord;
        }
        else {
            return TileCoord::Invalid();
        }
    });
}

bool Map::KeepConnectedPart(TileType type, int minTileCount, int maxTileCount) {
    int bestRoomNumber = 0;
    int roomCounter = 0;
    int bestRoomSize = minTileCount;
    while(TileCoord find = FindAvailableTile(TileType::FLOOR, *this)) {
        roomCounter++;
        int roomSize = RoomFlood4(roomCounter, TileType::FLOOR, find, *this);
        if (roomSize > bestRoomSize && roomSize < maxTileCount) {
            bestRoomSize = roomSize;
            bestRoomNumber = roomCounter;
        }
    }
    if(bestRoomNumber == 0) {
        return false;
    }
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).type == type && at(tileCoord).roomNumber != bestRoomNumber) {
            at(tileCoord).type = TileType::WALL;
        }
        at(tileCoord).roomNumber = 0;
    });
    return true;
}

TileCoord Map::FindMinimumBB() const {
    TileCoord result {width, height};
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber > 0) {
            result.x = std::min(result.x, tileCoord.x);
            result.y = std::min(result.y, tileCoord.y);
        }
    });
    return result;
}

TileCoord Map::FindMaximumBB() const {
    TileCoord result {0, 0};
    WalkTiles([&](const TileCoord & tileCoord) {
        if(at(tileCoord).roomNumber > 0) {
            result.x = std::max(result.x, tileCoord.x);
            result.y = std::max(result.y, tileCoord.y);
        }
    });
    return result;
}