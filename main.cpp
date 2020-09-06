#include <iostream>
#include "Dungeon/Dungeon.h"
#include "Dungeon/Room.h"

int main() {
    auto r = std::random_device();
    auto g = std::mt19937(r());
    Dungeon dungeon(220, 45);
    //Dungeon dungeon(350, 100);
    dungeon.GenerateDungeon(g);
    dungeon.Print();
    //Room room(30, 30);
    //room.Generate(0.0,0.1);
    //room.Print();
    return 0;
}
