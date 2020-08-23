#include <iostream>
#include "Dungeon/Dungeon.h"

int main() {
    Dungeon dungeon(80, 50);
    dungeon.Generate();
    dungeon.Print();
    return 0;
}
