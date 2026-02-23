//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#define PROCEDURALFOLIAGEGENERATOR_BEZIER_H
#include "spatial_coordinates.h"
#include <spatial/orienteering_path.h>
#include "ocad/ocad_coordinate.h"

namespace Spatial
{
    class PolyPath : public std::vector<Spatial2D>
    {
    public:

        bool IsClosed() const;
        bool IsPointInsideArea(Spatial2D point);
        bool IsPointOnPath(Spatial2D point);

        int FromBezier(OrienteeringPath& curve, int path_start);
    private:
        void CurveToPath(const Spatial2D& c0, const Spatial2D& c1, const Spatial2D& c2, const Spatial2D& c4);

    };
}


#endif //PROCEDURALFOLIAGEGENERATOR_BEZIER_H