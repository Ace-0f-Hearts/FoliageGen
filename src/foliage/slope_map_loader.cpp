//
// Created by ace on 2026-02-26.
//
#include <foliage/slope_map_loader.h>

SlopeMap SlopeMapLoader::Load(const HeightMap& map)
{
    CImg<> img(map.Dim().x, map.Dim().y);

    for (int i = 0; i < map.Dim().x; i++)
    {
        for (int j = 0; j < map.Dim().y; j++)
        {
            img(i,j) = map.SlopeAt({i,j});
        }
    }

    return {img};
}
