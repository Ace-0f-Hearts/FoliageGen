//
// Created by ace on 2026-02-26.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_H
#define PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_H
#include "height_map.h"

class SlopeMap : public TextureMap
{
public:
    SlopeMap(CImg<> const& map);
    [[nodiscard]] float SlopeAt(Coord2 const& coord) const;
};

#endif //PROCEDURALFOLIAGEGENERATOR_SLOPE_MAP_H