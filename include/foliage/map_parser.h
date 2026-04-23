//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_MAPPARSER_H
#define GENERATOR_MAPPARSER_H
#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <sstream>


#include <orienteering/map.h>
#include <orienteering/symbol_attribute.h>

class MapParser
{
public:
    MapParser(std::shared_ptr<Orienteering::OrienteeringMap> map);
    void Run(const std::filesystem::path  filename, const std::vector<SymbolAttribute>& attributes);
private:
    std::shared_ptr<Orienteering::OrienteeringMap> map_;
};




#endif //GENERATOR_MAPPARSER_H
