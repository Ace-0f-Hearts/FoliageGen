//
// Created by ace on 2026-03-17.
//

#include <orienteering/georeferencing.h>
#include <glm/mat2x2.hpp>
#include <glm/ext/matrix_transform.hpp>
Georeferencing::Georeferencing() :
    projected_ref_points_({0.f,0.f}),
    map_ref_points_({0.f,0.f}),
    declination_(0.0),
    grivation_(0.0),
    grivation_error_(0.0),
    combined_scale_factor_(1.0),
    auxiliary_scale_factor_(1.0),
    scale_denominator_(10000.0)
{
    UpdateTransformation();
}

void Georeferencing::UpdateTransformation()
{

    glm::mat4 transform(1.f);
    float scaling = combined_scale_factor_ * scale_denominator_ / 1000.0;

    transform = glm::translate(transform,glm::vec3(projected_ref_points_[0],projected_ref_points_[1],0.f));
    transform = glm::rotate(transform,-grivation_,glm::vec3(0.f,0.f,1.f));
    transform = glm::scale(transform,glm::vec3(scaling,-scaling,1.f));
    transform = glm::translate(transform,glm::vec3(-map_ref_points_[0],-map_ref_points_[1],0.f));


    to_projected_ = transform;
}

Spatial2D Georeferencing::ToProjectedCoords(Spatial2D coord) const
{
    glm::vec2 src = {coord[0],coord[1]};
    glm::vec2 dst = to_projected_ * src;
    return Spatial2D({dst.x,dst.y});
}

void Georeferencing::SetMapRefPoints(Spatial2D coord)
{
    map_ref_points_ = coord;
}

void Georeferencing::SetProjectedRefPoints(Spatial2D coords)
{
    projected_ref_points_ = coords;
}

void Georeferencing::SetDeclination(double value)
{
    declination_ = value;
}

void Georeferencing::SetGrivation(float value)
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
