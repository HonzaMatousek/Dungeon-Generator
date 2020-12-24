#pragma once

#include "../Dungeon/Map.h"

class Renderer {
public:
    virtual void Render(const Map & map) = 0;
};
