//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#include "object.h"

namespace Orienteering
{

    class PathObject : public Object
    {
    public:
        PathObject() = delete;
        PathObject(Symbol* symbol);

        ~PathObject() override = default;

        [[nodiscard]] bool Contains(const Spatial::Spatial2D& point) const;
        /**
         * Used for querying intersections with points. Depending on whether the path object designates an area or not, the intersection will account to the actual area of the path, or the area that's surrounded by it.
         * @param point
         * @return
         */
        [[nodiscard]]bool IsIntersecting(const Spatial::Spatial2D& point) const override ;


        /**
         * Computes a path consisting of poly lines from a series of coordinates representing a Bezier spline.
         * @param curve
         */
        void BuildCurve(std::vector<OcadCoordinate>& curve);
        void SetCurve(const std::vector<Spatial::Spatial2D>& points);
        void AppendPoint(const Spatial::Spatial2D& point);
        void RemovePoint(const Spatial::Spatial2D& point);
        void Clear();
    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H