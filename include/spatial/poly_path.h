//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#define PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#include "spatial_coordinates.h"
#include <spatial/orienteering_path.h>


namespace Spatial
{
    class PolyPath
    {
    public:

        void SetPoints(const std::vector<Spatial2D>& points);
        void AppendPoint(const Spatial2D& point);
        void RemovePoint(const Spatial2D& point);
        [[nodiscard]] bool IsClosed() const;
        [[nodiscard]] bool IsPointInsideArea(Spatial2D point) const;
        bool IsPointOnPath(Spatial2D point);


        int FromBezier(OrienteeringPath& curve, int path_start);
        [[nodiscard]] const std::vector<Spatial2D>& points() const;
        [[nodiscard]] std::vector<Spatial2D>& points();
        [[nodiscard]] int size() const;

    private:
        void CurveToPath(const Spatial2D& c0, const Spatial2D& c1, const Spatial2D& c2, const Spatial2D& c4);
        std::vector<Spatial2D> points_;
    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_BEZIER_H