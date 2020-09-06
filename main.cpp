#include <iostream>
#include "Dungeon/Dungeon.h"
#include "Dungeon/Room.h"

int main() {
    Dungeon dungeon(220, 45);
    //Dungeon dungeon(350, 100);
    dungeon.GenerateDungeon();
    dungeon.Print();
    //Room room(30, 30);
    //room.Generate(0.0,0.1);
    //room.Print();
    return 0;
}
