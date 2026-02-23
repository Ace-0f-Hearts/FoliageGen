//
// Created by ace on 2026-02-20.
//

#include <spatial/poly_path.h>

#include "utility/not_implemented_error.h"

bool Spatial::PolyPath::IsClosed() const
{
    return this->front() == this->back();
}

bool Spatial::PolyPath::IsPointInsideArea(Spatial2D point)
{
    auto inside = false;
    if (size() > 2)
    {
        auto prev = back();
        for (auto curr : *this)
        {
            if ( ((curr[1] > point[1]) != (prev[1] > point[1])) &&
                 (point[0] < (prev[0] - curr[0]) *
                  (point[1] - curr[1]) / (prev[1] - curr[1]) + curr[0]) )
            {
                inside = !inside;
            }

            prev = curr;
        }
    }
    return inside;
}

bool Spatial::PolyPath::IsPointOnPath(Spatial2D point)
{
    throw NotImplementedError();
}

int Spatial::PolyPath::FromBezier(OrienteeringPath& curve, int path_start)
{
    int path_end = curve.size() - 1;

    emplace_back(curve[path_start].coordinate());
    for (int i = path_start + 1; i < path_end; i++)
    {
        if (curve[i-1].IsCurveStart());
        {
            CurveToPath(curve[i-1].coordinate(),curve[i].coordinate(),curve[i+1].coordinate(),curve[i+2].coordinate());
            i += 2;
        }
        emplace_back(curve[i].coordinate());

        if (i < path_end && curve[i].IsHolePoint())
        {
            path_end = i;

        }
    }

    return path_end; // We return the index indicating the end of this distinct path
}

void Spatial::PolyPath::CurveToPath(const Spatial2D& c0, const Spatial2D& c1,const Spatial2D& c2, const Spatial2D& c3)
{
    auto c12 = (c1 + c2) / 2.f;

    auto start_to_end_len = c3.DistanceTo(c0);
    // Approximation of the curves length through chordal distance
    auto poly_line_len = c1.DistanceTo(c0) + c2.DistanceTo(c1) + c3.DistanceTo(c2);


    if (start_to_end_len < kBezier_max_segment_length && poly_line_len - start_to_end_len < kBezier_error)
    {
        this->emplace_back(c12);
    }
    else
    {
        Spatial2D c01 = (c0 + c1) * 0.5f;
        Spatial2D c23 = (c2 + c3) * 0.5f;
        Spatial2D c012 = (c01 + c12) * 0.5f;
        Spatial2D c123 = (c12 + c23) * 0.5f;
        Spatial2D c0123 = (c012 + c123) * 0.5f;
        
        CurveToPath(c0,c01,c012,c0123);
        CurveToPath(c0123,c123,c23,c3);
    }

}
