//
// Created by ace on 2026-02-23.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H
#define PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H



#include <CImg.h>


#include <foliage/texture_map.h>
#include "botanics_math/coord.h"
#include "spatial/spatial_coordinates.h"
using namespace cimg_library;
using namespace Spatial;

class HeightMap : public TextureMap
{


public:
    HeightMap() = delete;
    HeightMap(const CImg<>& height_map, float horizontal_scale = 1.f, float vertical_scale = 1.f);
    [[nodiscard]] float HeightAt(const Coord2& coord) const;
    [[nodiscard]] float SlopeAt(const Coord2& coord) const;
    [[nodiscard]] Spatial3D NormalVecAt(const Coord2& coord) const;

    [[nodiscard]] float vertical_scale() const;
    [[nodiscard]] float horizontal_scale() const;

private:
    [[nodiscard]] float HeightAtOrValue(const Coord2& coord, float value) const;

    float horizontal_scale_ = 1.f;
    float vertical_scale_ = 1.f;
};

#endif //PROCEDURALFOLIAGEGENERATOR_HEIGHT_MAP_H
