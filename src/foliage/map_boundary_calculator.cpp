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

void MapBoundaryCalculator::Write(std::filesystem::path output_path, double width, double height) const
{
    // First pass: compute bounds for scaling
    double xmin = 1e18, xmax = -1e18;
    double ymin = 1e18, ymax = -1e18;

    for (auto vit = map_shape_->finite_vertices_begin();
         vit != map_shape_->finite_vertices_end(); ++vit)
    {
        double x = vit->point().x();
        double y = vit->point().y();
        xmin = std::min(xmin, x);
        xmax = std::max(xmax, x);
        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);
    }

    double margin = 20.0;
    double sx = (width - 2 * margin) / (xmax - xmin);
    double sy = (height - 2 * margin) / (ymax - ymin);
    double scale = std::min(sx, sy);

    auto tx = [&](double x) { return margin + (x - xmin) * scale; };
    auto ty = [&](double y) { return margin + (y - ymin) * scale; };

    std::ofstream f(output_path, std::ofstream::out);
    f << R"(<?xml version="1.0" encoding="UTF-8"?>)" "\n";
    f << "<svg xmlns=\"http://www.w3.org/2000/svg\""
        << " width=\"" << width << "\" height=\"" << height << "\">\n";
    f << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";

    // Draw all input points
    for (auto vit = map_shape_->finite_vertices_begin();
         vit != map_shape_->finite_vertices_end(); ++vit)
    {
        double x = tx(vit->point().x());
        double y = ty(vit->point().y());
        f << "<circle cx=\"" << x << "\" cy=\"" << y
            << "\" r=\"1\" fill=\"steelblue\"/>\n";
    }

    // Draw REGULAR and SINGULAR boundary edges
    for (auto eit = map_shape_->alpha_shape_edges_begin();
         eit != map_shape_->alpha_shape_edges_end(); ++eit)
    {
        auto type = map_shape_->classify(*eit);
        if (type != Alpha_shape_2::REGULAR && type != Alpha_shape_2::SINGULAR) continue;

        CGAL::Segment_2 seg = map_shape_->segment(*eit);
        f << "<line"
            << " x1=\"" << tx(seg.source().x()) << "\""
            << " y1=\"" << ty(seg.source().y()) << "\""
            << " x2=\"" << tx(seg.target().x()) << "\""
            << " y2=\"" << ty(seg.target().y()) << "\""
            << " stroke=\"crimson\" stroke-width=\"1.5\"/>\n";
    }

    f << "</svg>\n";
}
