#pragma once

#include <string>
#include "Renderer.h"

class ImageRenderer : public Renderer {
    std::string outputFileName;
public:
    explicit ImageRenderer(const std::string & outputFileName);
    void Render(const Dungeon & dungeon) override;
};
