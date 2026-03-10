//
// Created by ace on 2026-02-26.
//
#include <foliage/slope_map_loader.h>

SlopeMap SlopeMapLoader::LoadHeightMap(const HeightMap& map)
{
    CImg<> img(map.Dim().x, map.Dim().y);

    for (int i = 0; i < map.Dim().y; i++)
    {
        for (int j = 0; j < map.Dim().x; j++)
        {
            img(j,i) = map.SlopeAt({j,i});
        }
    }

    return {img};
}
