//
// Created by ace on 2026-02-23.
//
#include <algorithm>
#include <filesystem>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>
#include <spatial/segmented_path.h>
#include <cassert>
#include "utility/not_implemented_error.h"


bool Spatial::SegmentedPath::IsPointInsideArea(const Spatial2D& point) const
{
    // Assumption is that: all areas/holes are fully inside the other areas that are to the left
    bool inside = false;


    for (const auto& path : paths())
    {
        if (path.IsPointInsideArea(point))
            inside = !inside;
    }

    return inside;
}

std::vector<Spatial::Spatial2D> Spatial::SegmentedPath::GetPoints() const
{
    std::vector<Spatial::Spatial2D> points;
    for (const auto& path : paths())
    {
        auto p = path.GetPoints();
        points.insert(points.end(), points.begin(), points.end());
    }
    return points;
}

bool Spatial::SegmentedPath::IsPointOnPath(const Spatial2D& point, float distance_threshold) const
{
    return std::ranges::any_of(paths().begin(), paths().end(), [&point, distance_threshold](const auto& path) {return path.IsPointOnPath(point,distance_threshold);});
}

void Spatial::SegmentedPath::FromBezier(std::vector<OcadCoordinate>& curve)
{
    size_t path_start = 0u;
    size_t size = curve.size();

    assert(size > 0);
    assert(size > path_start);

    while (path_start < size)
    {
        PolyPath path {};
        size_t path_end = path.FromBezier(curve, path_start);
        paths().push_back(path);

        assert(path.size() > 0);

        path_start = path_end + 1;
    }


    if (subdividing_)
        Subdivide();
}


const std::vector<Spatial::PolyPath>& Spatial::SegmentedPath::paths() const
{
    return paths_;
}

std::vector<Spatial::PolyPath>& Spatial::SegmentedPath::paths()
{
    return paths_;
}

void Spatial::SegmentedPath::Subdivide(float max_segment_length)
{
    auto new_paths = std::vector<Spatial::PolyPath>();
    for (auto & path : paths())
    {
        auto trimmed = SubdividePart(path,max_segment_length);

        new_paths.emplace_back(path);
        if (!trimmed.points().empty())
        {
            new_paths.emplace_back(trimmed);
        }

    }
    paths().swap(new_paths);

}

bool Spatial::SegmentedPath::Contains(const Spatial2D& point) const
{
    return std::ranges::any_of(paths().begin(), paths().end(), [&point](const auto& path)
    {
        return std::ranges::find(path.points().begin(), path.points().end(), point) != path.points().end();
    });
}

void Spatial::SegmentedPath::SetPoints(const std::vector<Spatial2D>& points)
{

    Clear();
    paths().emplace_back(points);

    if (subdividing_)
        Subdivide();
}

void Spatial::SegmentedPath::AppendPoint(const Spatial2D& point)
{
    paths().back().AppendPoint(point);

    if (subdividing_)
        SubdividePart(paths().back());
}

void Spatial::SegmentedPath::RemovePoint(const Spatial2D& point)
{
    for (auto& path : paths())
    {
        if (auto found = path.RemovePoint(point))
            break;
    }
}

void Spatial::SegmentedPath::Clear()
{
    paths().clear();
}

Spatial::PolyPath Spatial::SegmentedPath::SubdividePart(PolyPath& path, const float max_segment_length)
{

    auto rest = path.TrimToLength(max_segment_length);
    return {rest};
}

void Spatial::SegmentedPath::SetPoint(const Spatial2D& point)
{
    Clear();
    paths().emplace_back(point);
}

BoundingBox2D Spatial::SegmentedPath::ComputeBoundingBox() const
{
    Spatial2D max = paths().front().points().front();
    Spatial2D min = paths().front().points().front();
    for (auto path : paths())
    {
        for (auto point : path.points())
        {
            if (max[0] < point[0])
            {
                max[0] = point[0];
            }

            if (min[0] > point[0])
            {
                min[0] = point[0];
            }

            if (max[1] < point[1])
            {
                max[1] = point[1];
            }

            if (min[1] > point[1])
            {
                min[1] = point[1];
            }
        }
    }

    BoundingBox bounding_box {min, max};

    assert(
      std::ranges::all_of(paths(),[bounding_box](auto path)
      {
          return std::ranges::all_of(path.points(),[bounding_box](auto point)
          {
              return bounding_box.Contains(point);
          });
      })
    );

    return bounding_box;
}


Spatial::SegmentedPath::SegmentedPath(bool subdividing) : subdividing_(subdividing)
{
}

void Spatial::SegmentedPath::subdividing(bool enable)
{
    subdividing_ = enable;
}
