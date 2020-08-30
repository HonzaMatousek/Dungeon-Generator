#include "Room.h"

void Room::Generate(double minRoomRatio, double maxRoomRatio) {
    bool success = false;
    while(!success) {
        auto r = std::random_device();
        auto g = std::mt19937(r());
        Noise(g);
        for (int i = 0; i < 2; i++) {
            Blur(4, 4);
        }
        {
            int findX, findY;
            int roomCounter = 0;
            int bestRoomSize = (width - 2) * (height - 2) * minRoomRatio;
            int maxRoomSize = (width - 2) * (height - 2) * maxRoomRatio;
            int bestRoomNumber = 0;
            while (FindTile(0, TileType::FLOOR, findX, findY)) {
                roomCounter++;
                int roomSize = RoomFlood4(roomCounter, TileType::FLOOR, findX, findY);
                if (roomSize > bestRoomSize && roomSize < maxRoomSize) {
                    bestRoomSize = roomSize;
                    bestRoomNumber = roomCounter;
                }
            }
            if(bestRoomNumber == 0) {
                continue;
            }
            else {
                success = true;
            }
            for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                    auto &tile = tiles[row][col];
                    if (tile.roomNumber != bestRoomNumber && tile.type == TileType::FLOOR) {
                        tile.roomNumber = 0;
                        tile.type = TileType::WALL;
                    }
                }
            }
        }
    }
}
