//
// Created by ace on 2026-04-10.
//

#include <foliage/map_boundary_calculator.h>
MapBoundaryCalculator::MapBoundaryCalculator()
{

}

void MapBoundaryCalculator::ComputeAlphaShape(const std::vector<Spatial::Spatial2D>& spatial_coordinates)
{
    std::list<Point> points;
    for (auto coord : spatial_coordinates)
    {
        points.emplace_back(coord[0], coord[1]);
    }
    map_shape_.emplace(points.begin(), points.end());
    Alpha_iterator opt = map_shape_->find_optimal_alpha(1);


}

void MapBoundaryCalculator::ClassifyCoordiante(Spatial::Spatial2D coord)
{
    auto type = map_shape_->classify(Point{coord[0],coord[1]});

}

void MapBoundaryCalculator::Write(std::filesystem::path output_path)
{
    for (auto it = map_shape_->faces_begin(); it != map_shape_->faces_end(); ++it)
    {

    }
}
