//
// Created by ace on 2026-03-17.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H
#define PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H
#include "spatial/spatial_coordinates.h"

#include <glm/ext/matrix_double2x2.hpp>



using namespace Spatial;


class Georeferencing
{
public:
    Georeferencing();

    void UpdateTransformation();
    [[nodiscard]] Spatial2D ToProjectedCoords(Spatial2D coord) const;

    void SetMapRefPoints(Spatial2D coord);
    void SetProjectedRefPoints(Spatial2D coords);
    void SetDeclination(double value);
    void SetGrivation(float value);
    void SetCombinedScaleFactor(double value);
    void SetAuxiliaryScaleFactor(double value);
    void SetScaleDenominator(unsigned int value);
private:
    glm::mat2 to_projected_;


    Spatial2D projected_ref_points_;
    Spatial2D map_ref_points_;

    double declination_;
    float grivation_;
    double grivation_error_;
    double combined_scale_factor_;
    double auxiliary_scale_factor_;
    unsigned int scale_denominator_;
};

#endif //PROCEDURALFOLIAGEGENERATOR_GEOREFERENCING_H