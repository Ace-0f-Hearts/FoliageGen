//
// Created by ace on 2026-02-26.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_LOADER_H
#define PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_LOADER_H
#include "height_map.h"
#include "slope_map.h"

#endif //PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_LOADER_H

class SlopeMapLoader
{
public:
    SlopeMapLoader() = default;
    static SlopeMap Load(const HeightMap& map);
};
