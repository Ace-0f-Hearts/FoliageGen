//
// Created by ace on 2026-02-26.
//
#include <foliage/texture_map.h>

TextureMap::TextureMap(const CImg<>& map) : map_(map)
{
}

Dim2 TextureMap::Dim() const
{
    return Dim2(map_.height(), map_.width());
}

float TextureMap::At(Coord2 const& coord) const
{
    return map()(coord.x, coord.y);
}

CImg<> TextureMap::map() const
{
    return map_;
}
