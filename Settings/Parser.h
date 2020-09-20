#pragma once

#include <string>
#include <map>
#include <memory>

class Palette;
class FurnitureProvider;

class Parser {
    void RunPalette(std::istream & input, std::map<std::string, Palette> & palettes, const std::string & paletteName);
    void RunFurnitureThen(std::istream & input, const std::shared_ptr<FurnitureProvider> & furnitureProvider);
public:
    void Run(const std::string & fileName);
};
