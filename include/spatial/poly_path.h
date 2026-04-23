//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#define PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#include "spatial_coordinates.h"
#include <spatial/orienteering_path.h>
#include <ocad/ocad_coordinate.h>
#include <list>

namespace Spatial
{
    class PolyPath
    {
    public:
        PolyPath() = default;

        /** Used for initializing objects with multiple points, like paths and areas
         *
         * @param points
         */
        PolyPath(const std::vector<Spatial2D>& points);


        /** Used for initializing objects consisting of a single point.
         *
         * @param point
         */
        PolyPath(const Spatial2D& point);

        [[nodiscard]] std::list<Spatial2D> GetPoints() const;
        void SetPoints(const std::vector<Spatial2D>& points);
        void AppendPoint(const Spatial2D& point);
        bool RemovePoint(const Spatial2D& point);
        [[nodiscard]] bool IsClosed() const;
        [[nodiscard]] bool IsPointInsideArea(Spatial2D point) const;
        bool IsPointOnPath(Spatial2D point, float threshold_radius = 0.f) const;
        [[nodiscard]] float Length() const;
        [[nodiscard]] std::vector<Spatial2D> TrimToLength(float max_length);

        void Clear();

        size_t FromBezier(std::vector<OcadCoordinate>& curve, size_t path_start, float bezier_error = kBezier_error, float max_segment_length = kBezier_max_segment_length);
        [[nodiscard]] const std::vector<Spatial2D>& points() const;
        [[nodiscard]] std::vector<Spatial2D>& points();
        [[nodiscard]] size_t size() const;

    private:
        void CurveToPath(const Spatial2D& c0, const Spatial2D& c1, const Spatial2D& c2, const Spatial2D& c3, float bezier_error = kBezier_error , float
                                    max_segment_length = kBezier_max_segment_length);
        std::vector<Spatial2D> points_;
    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_BEZIER_H