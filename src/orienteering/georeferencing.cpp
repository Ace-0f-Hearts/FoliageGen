//
// Created by ace on 2026-03-17.
//

#include <orienteering/georeferencing.h>

void Georeferencing::UpdateTransform()
{
    matrix<double> transform;

    transform.
}

Spatial2D Georeferencing::ToProjectedCoords(Spatial2D coord)
{
}

void Georeferencing::SetDeclination(double value)
{
    declination_ = value;
}

void Georeferencing::SetGrivation(double value)
{
    grivation_ = value;
}

void Georeferencing::SetCombinedScaleFactor(double value)
{
    combined_scale_factor_ = value;
}

void Georeferencing::SetAuxiliaryScaleFactor(double value)
{
    auxiliary_scale_factor_ = value;
}

void Georeferencing::SetScaleDenominator(unsigned int value)
{
    scale_denominator_ = value;
}
