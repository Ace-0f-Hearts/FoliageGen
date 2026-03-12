//
// Created by ace on 2026-02-23.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H
#define PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H


#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0
#include <CImg.h>

#include <botanics_math/vec.h>

#include <foliage/texture_map.h>
#include "botanics_math/coord.h"

using namespace cimg_library;

class HeightMap : public TextureMap
{
public:
    HeightMap() = delete;
    HeightMap(const CImg<>& height_map, float horizontal_scale = 1.f, float vertical_scale = 1.f);
    [[nodiscard]] float HeightAt(Coord2 coord) const;
    [[nodiscard]] float SlopeAt(Coord2 coord) const;
    [[nodiscard]] Vec3 NormalVecAt(Coord2 coord) const;

    [[nodiscard]] float vertical_scale() const;
    [[nodiscard]] float horizontal_scale() const;

private:
    [[nodiscard]] float HeightAtOrValue(Coord2 coord, float value) const;

    float horizontal_scale_ = 1.f;
    float vertical_scale_ = 1.f;
};

#endif //PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H
