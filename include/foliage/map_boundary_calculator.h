//
// Created by ace on 2026-04-10.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_MAP_BOUNDARY_CALCULATOR_H
#define PROCEDURALFOLIAGEGENERATOR_MAP_BOUNDARY_CALCULATOR_H

#include <filesystem>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include "spatial/spatial_coordinate.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Gt;

typedef CGAL::Alpha_shape_vertex_base_2<Gt>          Vb;
typedef CGAL::Alpha_shape_face_base_2<Gt>            Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>  Tds;
typedef CGAL::Delaunay_triangulation_2<Gt,Tds>       Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>         Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator    Alpha_shape_edges_iterator;
typedef Gt::Point_2                                  Point;
typedef Alpha_shape_2::Alpha_iterator                Alpha_iterator;
typedef Gt::Segment_2                                        Segment;

class MapBoundaryCalculator
{
public:
    MapBoundaryCalculator();

    void ComputeAlphaShape(const std::list<Spatial::Spatial2D>& points);
    bool IsCoordinateInsideMap(Spatial::Spatial2D coord) const;
private:
    std::optional<Alpha_shape_2> map_shape_;
};


#endif //PROCEDURALFOLIAGEGENERATOR_MAP_BOUNDARY_CALCULATOR_H