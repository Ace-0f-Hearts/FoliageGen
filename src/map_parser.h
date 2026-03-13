//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_MAPPARSER_H
#define GENERATOR_MAPPARSER_H
#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <sstream>


#include "orienteering/map.h"

class MapParser
{
public:
    MapParser(std::shared_ptr<Orienteering::Map> map);
    void Run(const std::filesystem::path  filename);
private:
    std::shared_ptr<Orienteering::Map> map_;
};




#endif //GENERATOR_MAPPARSER_H
