//
// Created by honza on 13.09.20.
//

#include "OstreamRenderer.h"

OstreamRenderer::OstreamRenderer(std::ostream &ostream) : ostream(ostream) {}

void OstreamRenderer::Render(const Map & map) {
    for (int row = 0; row < map.GetHeight(); row++) {
        for (int col = 0; col < map.GetWidth(); col++) {
            switch(map.at(col, row).type) {
                case TileType::WALL:
                    if(map.at(col, row).roomNumber == -1) {
                        ostream << '@';
                    }
                    else if(map.CountNeighbors8({ col, row }, TileType::WALL, true) > 7) {
                        ostream << ' ';
                    }
                    else {
                        ostream << "█";
                    }
                    break;
                case TileType::FLOOR:
                    //std::cout << (char)('0' + tiles[row][col].roomNumber);
                    if(map.at(col, row).roomNumber == 0) {
                        ostream << "\033[38;5;" << (30 + (map.at(col, row).roomNumber % 200)) << "m-\033[0m";
                    }
                    else {
                        ostream << "\033[38;5;" << (30 + (map.at(col, row).roomNumber % 200)) << "m.\033[0m";
                    }
                    break;
                case TileType::DOOR:
                    ostream << '#';
                    break;
                case TileType::MONSTER:
                    ostream << "☻";
                    break;
                case TileType::CHEST:
                    ostream << "☐";
                    break;
                case TileType::MASK:
                    ostream << '@';
                    break;
                case TileType::START:
                    ostream << '$';
                    break;
                case TileType::FINISH:
                    ostream << '^';
                    break;
            }
        }
        ostream << std::endl;
    }
}
