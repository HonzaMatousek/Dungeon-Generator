#pragma once

#include <string>
#include <vector>
#include "Renderer.h"

class ImageRenderer : public Renderer {
    std::string outputFileName;
    std::vector<char> tiles;
public:
    explicit ImageRenderer(const std::string & outputFileName);
    void Render(const Dungeon & dungeon) override;
};
