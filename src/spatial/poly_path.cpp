//
// Created by ace on 2026-02-20.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <spatial/poly_path.h>

#include "utility/not_implemented_error.h"

bool Spatial::PolyPath::IsClosed() const
{
    return points_.front() == points_.back();
}


bool Spatial::PolyPath::IsPointInsideArea(Spatial2D point) const
{
    auto inside = false;
    if (points_.size() > 2)
    {
        auto prev = points_.back();
        for (auto curr : points_)
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

bool Spatial::PolyPath::IsPointOnPath(Spatial2D point, float threshold_radius) const
{
    auto prev = points_.front();
    for (size_t point_idx = 1; point_idx < points_.size(); point_idx++)
    {
        auto next = points_[point_idx];

        Spatial2D pa = point - prev;
        Spatial2D ba = next - prev;

        float h = std::clamp(Dot(pa,ba)/Dot(ba,ba),0.0f,1.0f);
        if (Spatial::Length(pa - ba * h) < threshold_radius)
            return true;
    }
    return false;

}

size_t Spatial::PolyPath::FromBezier(std::vector<OcadCoordinate>& curve, size_t path_start = 0, float bezier_error, float max_segment_length)
{
    // assert(curve.size() > 1);

    auto path_end = curve.size();

    points_.emplace_back(curve[path_start].coordinate());
    for (size_t i = path_start + 1; i < path_end; i++)
    {
        if (curve[i-1].IsCurveStart())
        {

            assert(i+2  <= path_end);
            CurveToPath(curve[i-1].coordinate(),curve[i].coordinate(),curve[i+1].coordinate(),curve[i+2].coordinate(),bezier_error,max_segment_length);
            i += 2;

        }
        points_.emplace_back(curve[i].coordinate());

        if (i < path_end && curve[i].IsHolePoint())
        {
            path_end = i;

        }
    }

    return path_end; // We return the index indicating the end of this distinct path
}

void Spatial::PolyPath::CurveToPath(const Spatial2D& c0, const Spatial2D& c1, const Spatial2D& c2, const Spatial2D& c3, float bezier_error, float max_segment_length)
{
    auto c12 = (c1 + c2) / 2.f;

    auto start_to_end_len = c3.DistanceTo(c0);
    // Approximation of the curves length through chordal distance
    auto poly_line_len = c1.DistanceTo(c0) + c2.DistanceTo(c1) + c3.DistanceTo(c2);


    if (start_to_end_len < max_segment_length && poly_line_len - start_to_end_len < bezier_error)
    {
        points_.emplace_back(c12);
    }
    else
    {
        const Spatial2D c01 = (c0 + c1) * 0.5f;
        const Spatial2D c23 = (c2 + c3) * 0.5f;
        const Spatial2D c012 = (c01 + c12) * 0.5f;
        const Spatial2D c123 = (c12 + c23) * 0.5f;
        const Spatial2D c0123 = (c012 + c123) * 0.5f;
        
        CurveToPath(c0,c01,c012,c0123, bezier_error, max_segment_length);
        CurveToPath(c0123,c123,c23,c3, bezier_error, max_segment_length);
    }

}

const std::vector<Spatial::Spatial2D>& Spatial::PolyPath::points() const
{
    return points_;
}

size_t Spatial::PolyPath::size() const
{
    return points_.size();
}

void Spatial::PolyPath::SetPoints(const std::vector<Spatial2D>& points)
{
    points_ = points;
}

void Spatial::PolyPath::AppendPoint(const Spatial2D& point)
{
    points_.emplace_back(point);
}

bool Spatial::PolyPath::RemovePoint(const Spatial2D& point)
{
    auto it = std::ranges::find(points_.begin(), points_.end(),point);
    auto found = it != points_.end();
    if (found)
        points_.erase(it);
    return found;
}

std::vector<Spatial::Spatial2D>& Spatial::PolyPath::points()
{
    return points_;
}

Spatial::PolyPath::PolyPath(const std::vector<Spatial2D>& points) : points_(points)
{
}

float Spatial::PolyPath::Length() const
{
    float length = 0;
    for (size_t i = 0; i < points_.size() - 1; i++)
        length += points_[i].DistanceTo(points_[i+1]);

    return length;
}

Spatial::PolyPath::PolyPath(const Spatial2D& point)
{
    Clear();
    points_.push_back(point);
}

void Spatial::PolyPath::Clear()
{
    points_.clear();
}


std::vector<Spatial::Spatial2D> Spatial::PolyPath::TrimToLength(float max_length)
{
    float current_length = 0;
    auto idx = 0u;
    for (; idx < points_.size() - 1 && current_length <= max_length; idx++)
    {
        current_length += points_[idx].DistanceTo(points_[idx+1]);

    }

    std::vector<Spatial2D> rest = {points().begin() + (idx + 1), points().end()};
    points_ = {points_.begin(), points_.begin() + (idx + 1)};

    return rest;
}
