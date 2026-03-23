//
// Created by ace on 2026-03-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_H
#define PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_H
#include "seed.h"
#include "texture_map.h"

using namespace cimg_library;
class FoliageMap : public TextureMap
{
    friend class FoliageMapWriter;
public:
    FoliageMap() = delete;
    FoliageMap(CImg<> const& map) : TextureMap(map) {}
};

#endif //PROCEDURALFOLIAGEGENERATOR_FOLIAGE_MAP_H