//
// Created by ace on 2026-02-25.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_LOADER_H
#define PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_LOADER_H
#include <filesystem>

#include "height_map.h"

class HeightMapLoader
{
    public:
    HeightMapLoader() = delete;
    static HeightMap LoadHeightMap(std::filesystem::path);
};

#endif //PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_LOADER_H