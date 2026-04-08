//
// Created by ace on 2026-02-26.
//

#include <foliage/slope_map.h>

SlopeMap::SlopeMap(CImg<> const& map) : TextureMap(map)
{
}

float SlopeMap::SlopeAt(Coord2 const& coord) const
{
    return At_(coord);
}
