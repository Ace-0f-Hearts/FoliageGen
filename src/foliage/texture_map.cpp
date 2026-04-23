//
// Created by ace on 2026-02-26.
//
#include <cassert>
#include <foliage/texture_map.h>
TextureMap::TextureMap(const CImg<>& map) : map_(map)
{
}

Dim2 TextureMap::Dim() const
{
    return Dim2(map_.height(), map_.width());
}

float TextureMap::At_(Coord2 const& coord) const
{
    float value = map_(coord.x, coord.y);

    assert(!std::isinf(value));
    assert(!std::isnan(value));

    return value;
}

void TextureMap::At_(Coord2 const& coord, float value)
{
    assert(!std::isinf(value));
    assert(!std::isnan(value));
    map_(coord.x, coord.y) = value;
}


const CImg<>& TextureMap::map() const
{
    return map_;
}

CImg<>& TextureMap::map()
{
    return map_;
}

int TextureMap::Width() const
{
    return map_.width();
}

int TextureMap::Height() const
{
    return map_.height();
}
