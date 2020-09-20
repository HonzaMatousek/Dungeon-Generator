//
// Created by honza on 06.09.20.
//

#include "FurnitureStyle.h"
#include "FurnitureProvider.h"

std::shared_ptr<FurnitureProvider> FurnitureStyle::getSubFurnitures() const {
    return subFurnitures;
}

FurnitureStyle::FurnitureStyle() : subFurnitures(std::make_shared<FurnitureProvider>()) {}

void FurnitureStyle::FurnitureRoom(Dungeon &dungeon, int roomNumber, std::mt19937 &gen) const {
    if(!subFurnitures->Empty()) {
        subFurnitures->RandomFurnitureStyle(gen)->FurnitureRoom(dungeon, roomNumber, gen);
    }
}
