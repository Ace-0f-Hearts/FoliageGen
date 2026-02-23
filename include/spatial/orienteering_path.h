//
// Created by ace on 2026-02-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_BEZIER_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_BEZIER_H
#include <vector>

#include "../ocad/ocad_coordinate.h"

namespace Spatial
{
    constexpr static float kBezier_error = 0.005f;
    constexpr static float kBezier_max_segment_length = 1.0f;


    /**
    * An intermediate type used in  converting paths with common coordinate and point descriptions used in orienteering applications, to a more suitable format for generation.
    */
    class OrienteeringPath : public std::vector<OcadCoordinate>
    {
    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_BEZIER_H