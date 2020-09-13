#include "ImageRenderer.h"
#include <fstream>

ImageRenderer::ImageRenderer(const std::string & outputFileName) : outputFileName(outputFileName) {
    std::ifstream tilesFile("dungeon-tiles.tga", std::ios::binary);
    char TGAheader[18];
    tilesFile.read(TGAheader, sizeof(TGAheader));
    tiles.resize(64 * 64 * 3);
    tilesFile.read(tiles.data(), tiles.size());
}

void ImageRenderer::Render(const Dungeon &dungeon) {
    std::ofstream outputFile(outputFileName, std::ios::binary);
    unsigned char TGAheader[18] = {0,0,2, 0,0, 0,0, 16, 0,0, 0,0, (unsigned char)((16 * dungeon.getWidth()) & 0xFFu), (unsigned char)((16 * dungeon.getWidth()) >> 8u), (unsigned char)((16 * dungeon.getHeight()) & 0xFFu), (unsigned char)((16 * dungeon.getHeight()) >> 8u), 24, 0x20};
    outputFile.write((char *)TGAheader, sizeof(TGAheader));
    for (int row = 0; row < dungeon.getHeight(); row++) {
        for(int line = 0; line < 16; line++) {
            for (int col = 0; col < dungeon.getWidth(); col++) {
                int spriteX = 0;
                int spriteY = 0;
                switch (dungeon.at(col, row).type) {
                    case TileType::WALL:
                        if (dungeon.CountNeighbors8({col, row}, TileType::WALL, true) > 7) {
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
                }
                outputFile.write(tiles.data() + ((spriteY + line) * 64 + spriteX) * 3, 16 * 3);
            }
        }
    }
}
