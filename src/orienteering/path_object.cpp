//
// Created by ace on 2026-03-01.
//
#include <algorithm>
#include <orienteering/path_object.h>
#include <ranges>

PathObject::PathObject(std::shared_ptr<Symbol> symbol) : Object(symbol)
{
    switch (symbol->type())
    {
        case AreaS:
            type_ = AreaO;
        break;
        case PathS:
            type_ = PathO;
        break;
        default:
        throw std::logic_error("Type of path object is corrupted.");
    }
}



bool PathObject::Contains(const Spatial::Spatial2D& point) const
{
    return coordinates_.Contains(point);
}


bool PathObject::IsIntersecting(const Spatial::Spatial2D& point) const
{
    auto intersecting = false;
    switch (type())
    {
    case AreaO:
        intersecting = coordinates_.IsPointInsideArea(point);
        break;
    case PathO:
        intersecting = coordinates_.IsPointOnPath(point,symbol_->GetRadiusOfInfluence());
        break;
    default:
        throw std::logic_error("Type of path object is corrupted.");
    }

    return intersecting;
}

void PathObject::BuildCurve(std::vector<ObjectCoordinate>& curve)
{
    coordinates_.FromBezier(curve);
    UpdateBoundingBox();
}

void PathObject::SetCurve(const std::vector<Spatial::Spatial2D>& points)
{
    coordinates_.SetPoints(points);
    UpdateBoundingBox();
}

void PathObject::AppendPoint(const Spatial::Spatial2D& point)
{
    coordinates_.AppendPoint(point);
    UpdateBoundingBox();
}

void PathObject::RemovePoint(const Spatial::Spatial2D& point)
{
    coordinates_.RemovePoint(point);
    UpdateBoundingBox();
}

void PathObject::Clear()
{
    coordinates_.Clear();
}

