//
// Created by ace on 2026-03-17.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H
#define PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H
#include "spatial/spatial_coordinates.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>


using namespace boost::numeric::ublas;
using namespace boost::geometry::strategy::transform;
using namespace Spatial;


class Georeferencing
{

    void UpdateTransform();
    Spatial2D ToProjectedCoords(Spatial2D coord);

    void SetDeclination(double value);
    void SetGrivation(double value);
    void SetCombinedScaleFactor(double value);
    void SetAuxiliaryScaleFactor(double value);
    void SetScaleDenominator(unsigned int value);
public:

    matrix<double> to_projection;

    Spatial2D projected_ref_points;

    double declination_;
    double grivation_;
    double grivation_error_;
    double combined_scale_factor_;
    double auxiliary_scale_factor_;
    unsigned int scale_denominator_;
};

#endif //PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H