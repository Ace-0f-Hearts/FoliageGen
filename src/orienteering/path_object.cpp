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
    return false;
}

bool PathObject::Contains(const Spatial2D& point) const
{
    return Contains(point);
}


bool PathObject::IsIntersecting(const Spatial2D& point)
{
    return false;
}

void PathObject::BuildCurve(std::vector<OcadCoordinate>& curve)
{
    coordinates_.FromBezier(curve);
}

void PathObject::SetCurve(const std::vector<Spatial2D>& points)
{
    coordinates_.SetPoints(points);
}

void PathObject::AppendPoint(const Spatial2D& point)
{
    coordinates_.AppendPoint(point);
}

void PathObject::RemovePoint(const Spatial2D& point)
{
    coordinates_.RemovePoint(point);
}

void PathObject::Clear()
{
    coordinates_.Clear();
}

