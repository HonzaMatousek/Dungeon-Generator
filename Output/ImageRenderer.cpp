#include "ImageRenderer.h"
#include <fstream>

ImageRenderer::ImageRenderer(const std::string & outputFileName) : outputFileName(outputFileName) {
    std::ifstream tilesFile("dungeon-tiles.tga", std::ios::binary);
    char TGAheader[18];
    tilesFile.read(TGAheader, sizeof(TGAheader));
    tiles.resize(64 * 64 * 3);
    tilesFile.read(tiles.data(), tiles.size());
}

void ImageRenderer::Render(const Map & map) {
    std::ofstream outputFile(outputFileName, std::ios::binary);
    unsigned char TGAheader[18] = {0,0,2, 0,0, 0,0, 16, 0,0, 0,0, (unsigned char)((16 * map.GetWidth()) & 0xFFu), (unsigned char)((16 * map.GetWidth()) >> 8u), (unsigned char)((16 * map.GetHeight()) & 0xFFu), (unsigned char)((16 * map.GetHeight()) >> 8u), 24, 0x20};
    outputFile.write((char *)TGAheader, sizeof(TGAheader));
    for (int row = 0; row < map.GetHeight(); row++) {
        for(int line = 0; line < 16; line++) {
            for (int col = 0; col < map.GetWidth(); col++) {
                int spriteX = 0;
                int spriteY = 0;
                switch (map.at(col, row).type) {
                    case TileType::WALL:
                        if (map.CountNeighbors8({col, row}, TileType::WALL, true) > 7) {
                            spriteX = 0;
                            spriteY = 48;
                        }
                        else {
                            spriteX = 0;
                            spriteY = 0;
                        }
                        break;
                    case TileType::FLOOR:
                        spriteX = 0;
                        spriteY = 16;
                        break;
                    case TileType::DOOR:
                        spriteX = 0;
                        spriteY = 32;
                        break;
                    case TileType::MONSTER:
                        spriteX = 16;
                        spriteY = 0;
                        break;
                    case TileType::CHEST:
                        spriteX = 16;
                        spriteY = 16;
                        break;
                    case TileType::MASK:
                        spriteX = 16;
                        spriteY = 48;
                        break;
                    case TileType::START:
                        spriteX = 32;
                        spriteY = 0;
                        break;
                    case TileType::FINISH:
                        spriteX = 48;
                        spriteY = 0;
                        break;
                }
                outputFile.write(tiles.data() + ((spriteY + line) * 64 + spriteX) * 3, 16 * 3);
            }
        }
    }
}
