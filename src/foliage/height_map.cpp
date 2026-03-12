//
// Created by ace on 2026-02-23.
//
#include <cassert>
#include <iostream>
#include <foliage/height_map.h>

HeightMap::HeightMap(const CImg<>& height_map, const float horizontal_scale, const float vertical_scale): TextureMap(height_map), horizontal_scale_(horizontal_scale), vertical_scale_(vertical_scale)
{
    if (height_map.width() < 1 || height_map.height() < 1)
        throw std::invalid_argument("Loaded height map is empty");

};

float HeightMap::HeightAt(const Coord2 coord) const
{
    assert(coord.x >= 0 && coord.y >= 0 && coord.x < map().width() && coord.y < map().height());

    return At(coord) * vertical_scale();
}

Vec3 HeightMap::NormalVecAt(const Coord2 coord) const
{
    const auto h11 = HeightAt(coord);
    const auto h01 = HeightAtOrValue(coord - Coord2{1,0},h11);
    const auto h21 = HeightAtOrValue(coord + Coord2{1,0},h11);
    const auto h10 = HeightAtOrValue(coord - Coord2{0,1},h11);
    const auto h12 = HeightAtOrValue(coord + Coord2{0,1},h11);

    const auto du = Normalize(Vec3(2.f,h21 - h01,0.f));
    const auto dv = Normalize(Vec3(0.f,h12 - h10,2.f));
    return Normalize(Cross(du,dv));
}

float HeightMap::SlopeAt(const Coord2 coord) const
{
    const Vec3 normal = NormalVecAt(coord);
    const float dot = Dot(normal,kUp) / (Normal(normal) * Normal(kUp));
    const float result = std::fabs(std::acosf(dot) - static_cast<float>(M_PI / 2.0f));
    return result;
}

float HeightMap::HeightAtOrValue(const Coord2 coord, const float value) const
{
    if (coord.x < 0 || coord.y < 0 || coord.x > map().width() || coord.y > map().height())
        return value;

    return At(coord) * vertical_scale();
}

float HeightMap::vertical_scale() const
{
    return vertical_scale_;
}

float HeightMap::horizontal_scale() const
{
    return horizontal_scale_;
}


