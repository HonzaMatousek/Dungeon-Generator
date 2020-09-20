#pragma once

#include "FurnitureStyle.h"

enum MazeType {
    MAZE,
    DIRECT,
};

class MazeFurniture : public FurnitureStyle {
    MazeType mazeType;
public:
    MazeFurniture(MazeType mazeType);

    void FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const override;

    std::unique_ptr<FurnitureStyle> Clone() const override;
};
