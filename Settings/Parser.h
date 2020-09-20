#pragma once

#include <string>
#include <map>

class Palette;

class Parser {
    void RunPalette(std::istream & input, std::map<std::string, Palette> & palettes, const std::string & paletteName);
public:
    void Run(const std::string & fileName);
};
