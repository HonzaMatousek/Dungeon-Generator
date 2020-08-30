#include <iostream>
#include "Dungeon/Dungeon.h"

int main() {
    Dungeon dungeon(220, 45);
    dungeon.Generate();
    dungeon.Print();
    return 0;
}
