//
// Created by ace on 2026-04-10.
//

#include <loguru.hpp>
#include <foliage/map_boundary_calculator.h>

template <class OutputIterator>
void alpha_edges(const Alpha_shape_2& A, OutputIterator out)
{
    Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin(),
                               end = A.alpha_shape_edges_end();
    for (; it != end; ++it)
        *out++ = A.segment(*it);
}

MapBoundaryCalculator::MapBoundaryCalculator()
{
}

void MapBoundaryCalculator::ComputeAlphaShape(const std::list<Spatial::Spatial2D>& spatial_coordinates)
{
    std::list<Point> points;
    for (auto coord : spatial_coordinates)
    {
        points.emplace_back(coord[0], coord[1]);
    }
    map_shape_.emplace(points.begin(), points.end(), Gt::FT(10000));

    map_shape_->set_alpha(*map_shape_->find_optimal_alpha(1) * 3);
    std::vector<Segment> segments;
    alpha_edges(map_shape_.value(), std::back_inserter(segments));
}

bool MapBoundaryCalculator::IsCoordinateInsideMap(Spatial::Spatial2D coord) const
{
    auto type = map_shape_->classify(Point{coord[0], coord[1]});
    return type != Alpha_shape_2::EXTERIOR;
}


