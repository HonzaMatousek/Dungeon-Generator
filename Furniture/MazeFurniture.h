#pragma once

#include "FurnitureStyle.h"

enum MazeType {
    MAZE,
    DIRECT,
};

class MazeFurniture : public FurnitureStyle {
    MazeType mazeType;
public:
    explicit MazeFurniture(MazeType mazeType);

    void FurnitureRoom(Room & room, std::mt19937 &gen) const override;

    [[ nodiscard ]]
    std::unique_ptr<FurnitureStyle> Clone() const override;
};
