//
// Created by ace on 2026-02-23.
//
#include <spatial/path_collection.h>

bool Spatial::PathCollection::IsPointInsideArea(const Spatial2D& point) const
{
    // Assumption is that: all areas/holes are fully inside the other areas that are to the left
    bool inside = false;
    for (auto path : *this)
    {
        if (path.IsPointInsideArea(point))
            inside = !inside;
    }
    return inside;
}

void Spatial::PathCollection::FromBezier(OrienteeringPath& curve)
{
    int path_start = 0;
    int size = curve.size();

    while (path_start < size)
    {
        PolyPath path {};
        int path_end = path.FromBezier(curve, path_start);
        push_back(path);
        path_start = path_end + 1;
    }

}
