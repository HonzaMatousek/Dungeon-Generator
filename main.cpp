#include <iostream>
#include "Dungeon/Dungeon.h"
#include "Dungeon/Room.h"

int main() {
    //Dungeon dungeon(220, 45);
    //dungeon.Generate();
    //dungeon.Print();
    Room room(30, 30);
    room.Generate();
    room.Print();
    return 0;
}
