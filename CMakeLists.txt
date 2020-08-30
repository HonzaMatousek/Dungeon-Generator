cmake_minimum_required(VERSION 3.17)
project(dungeon_gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(dungeon_gen main.cpp Dungeon/Dungeon.cpp Dungeon/Dungeon.h Dungeon/Tile.cpp Dungeon/Tile.h Dungeon/Room.cpp Dungeon/Room.h)