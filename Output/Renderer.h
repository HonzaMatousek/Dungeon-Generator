#pragma once

#include "../Dungeon/Dungeon.h"

class Renderer {
public:
    virtual void Render(const Dungeon & dungeon) = 0;
};
