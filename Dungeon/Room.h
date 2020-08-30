#pragma once

#include "Dungeon.h"

class Room : public Dungeon {
public:
    using Dungeon::Dungeon;
    void Generate() override;
};
