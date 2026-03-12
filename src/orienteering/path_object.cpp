//
// Created by ace on 2026-03-01.
//
#include <algorithm>
#include <orienteering/path_object.h>
#include <ranges>

PathObject::PathObject(Symbol* symbol) : Object(symbol)
{
    type_ = PathO;
}

bool PathObject::IsClosed() const
{
    return coordinates_.IsClosed();

}

bool PathObject::Contains(const Spatial::Spatial2D& point) const
{
    return coordinates_.Contains(point);
}


bool PathObject::IsIntersecting(const Spatial::Spatial2D& point)
{
    auto intersecting = false;
    switch (type())
    {
    case AreaO:
        intersecting = coordinates_.IsPointInsideArea(point);
        break;
    case PathO:
        intersecting = coordinates_.IsPointOnPath(point,distance_threshold_);
        break;
    default:
        throw std::logic_error("Type of path object is corrupted.");
    }

    return intersecting;
}

void PathObject::BuildCurve(std::vector<OcadCoordinate>& curve)
{
    coordinates_.FromBezier(curve);
}

void PathObject::SetCurve(const std::vector<Spatial::Spatial2D>& points)
{
    coordinates_.SetPoints(points);
}

void PathObject::AppendPoint(const Spatial::Spatial2D& point)
{
    coordinates_.AppendPoint(point);
}

void PathObject::RemovePoint(const Spatial::Spatial2D& point)
{
    coordinates_.RemovePoint(point);
}

void PathObject::Clear()
{
    coordinates_.Clear();
}

