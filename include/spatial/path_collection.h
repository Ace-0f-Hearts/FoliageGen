//
// Created by ace on 2026-02-23.

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#include <vector>

#include "poly_path.h"


namespace Spatial
{
    /**
    * Holds a collection of paths that are used to handle areas which might have holes in them
    */
    class PathCollection : public std::pmr::vector<Spatial::PolyPath>
    {
    public:
        [[nodiscard]] bool IsPointInsideArea(const Spatial2D& point) const;
        void FromBezier(std::vector<OcadCoordinate>& curve);
    };
}



#endif //PROCEDURALFOLIAGEGENERATOR_PATH_PART_H