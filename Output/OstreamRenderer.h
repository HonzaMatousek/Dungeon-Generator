#pragma once

#include <ostream>
#include "Renderer.h"

class OstreamRenderer : public Renderer {
    std::ostream & ostream;
public:
    explicit OstreamRenderer(std::ostream & ostream);
    void Render(const Dungeon & dungeon) override;
};
