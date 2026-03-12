//
// Created by ace on 2026-02-23.
//
#include <algorithm>
#include <filesystem>
#include <boost/beast/http/field.hpp>
#include <spatial/path_collection.h>

#include "utility/not_implemented_error.h"


bool Spatial::PathCollection::IsPointInsideArea(const Spatial2D& point) const
{
    // Assumption is that: all areas/holes are fully inside the other areas that are to the left
    bool inside = false;
    for (auto path : paths())
    {
        if (path.IsPointInsideArea(point))
            inside = !inside;
    }
    return inside;
}

void Spatial::PathCollection::FromBezier(std::vector<OcadCoordinate>& curve)
{
    size_t path_start = 0u;
    size_t size = curve.size();

    while (path_start < size)
    {
        PolyPath path {};
        size_t path_end = path.FromBezier(curve, path_start);
        paths().push_back(path);
        path_start = path_end + 1;
    }

    Subdivide();
}

bool Spatial::PathCollection::IsPointOnPath(const Spatial2D& point) const
{
    throw NotImplementedError();
}

const std::vector<Spatial::PolyPath>& Spatial::PathCollection::paths() const
{
    return paths_;
}

std::vector<Spatial::PolyPath>& Spatial::PathCollection::paths()
{
    return paths_;
}

void Spatial::PathCollection::Subdivide(float max_segment_length)
{
    auto new_paths = std::vector<Spatial::PolyPath>();
    for (auto & path : paths())
    {
        auto trimmed = SubdividePart(path,max_segment_length);

        new_paths.emplace_back(path);
        if (!trimmed.points().empty())
            new_paths.emplace_back(trimmed);
    }
}

bool Spatial::PathCollection::Contains(const Spatial2D& point) const
{
    return std::ranges::any_of(paths().begin(), paths().end(), [&point](const auto& path)
    {
        return std::ranges::find(path.points().begin(), path.points().end(), point) != path.points().end();
    });
}

void Spatial::PathCollection::SetPoints(const std::vector<Spatial2D>& points)
{

    Clear();
    paths().emplace_back(points);
    Subdivide();
}

void Spatial::PathCollection::AppendPoint(const Spatial2D& point)
{
    paths().back().AppendPoint(point);
    SubdividePart(paths().back());
}

void Spatial::PathCollection::RemovePoint(const Spatial2D& point)
{
    for (auto& path : paths())
    {
        if (auto found = path.RemovePoint(point))
            break;
    }
}

void Spatial::PathCollection::Clear()
{
    paths().clear();
}

Spatial::PolyPath Spatial::PathCollection::SubdividePart(PolyPath& path, const float max_segment_length)
{

    auto rest = path.TrimToLength(max_segment_length);
    return {rest};
}

void Spatial::PathCollection::SetPoint(const Spatial2D& point)
{
    Clear();
    paths().emplace_back(point);
}
