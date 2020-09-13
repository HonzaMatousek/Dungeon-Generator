#include "ImageRenderer.h"
#include <fstream>

ImageRenderer::ImageRenderer(const std::string & outputFileName) : outputFileName(outputFileName) {}

void ImageRenderer::Render(const Dungeon &dungeon) {
    std::ofstream outputFile(outputFileName, std::ios::binary);
    unsigned char TGAheader[18] = {0,0,2, 0,0, 0,0, 16, 0,0, 0,0, (unsigned char)(dungeon.getWidth() & 0xFFu), (unsigned char)(dungeon.getWidth() >> 8u), (unsigned char)(dungeon.getHeight() & 0xFFu), (unsigned char)(dungeon.getHeight() >> 8u), 24, 0x20};
    outputFile.write((char *)TGAheader, sizeof(TGAheader));
    dungeon.WalkTiles([&] (const TileCoord & tileCoord) {
        switch(dungeon.at(tileCoord).type) {
            case TileType::WALL:
                if(dungeon.at(tileCoord).roomNumber == -1) {
                    outputFile.write("\xcc\xcc\xff", 3);
                }
                else if(dungeon.CountNeighbors8(tileCoord, TileType::WALL, true) > 7) {
                    outputFile.write("\xff\xff\xff", 3);
                }
                else {
                    outputFile.write("\x00\x00\x00", 3);
                }
                break;
            case TileType::FLOOR:
                outputFile.write("\xcc\xcc\xcc", 3);
                break;
            case TileType::DOOR:
                outputFile.write("\x99\x99\x99", 3);
                break;
            case TileType::MONSTER:
                outputFile.write("\x00\x00\xff", 3);
                break;
            case TileType::CHEST:
                outputFile.write("\xee\xee\x00", 3);
                break;
        }
    });
}
