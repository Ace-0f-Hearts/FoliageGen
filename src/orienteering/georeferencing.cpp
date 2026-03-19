//
// Created by ace on 2026-03-17.
//

#include <orienteering/georeferencing.h>
#include <boost/qvm/mat_operations.hpp>

Georeferencing::Georeferencing() :
scale_denominator_(1000),
combined_scale_factor_(1.0),
auxiliary_scale_factor_(1.0),
declination_(0.0),
grivation_(0.0),
grivation_error_(0.0),
map_ref_points_({0.f,0.f}),
projected_ref_points_({0.f,0.f})
{
    UpdateTransformation();
}

void Georeferencing::UpdateTransformation()
{
    boost::qvm::mat<double,3,3> transform = {1,0,0,0,1,0,0,0,1};
    double scale = combined_scale_factor_ * scale_denominator_ / 1000.0;

    boost::qvm::mat<double,3,3> proj_ref_translate = {1,0,projected_ref_points_[0],0,1,projected_ref_points_[1],0,0,1};
    boost::qvm::mat<double,3,3> scale_mat = {scale,0,0,-scale,0,0,0,0,1};
    boost::qvm::mat<double,3,3> map_ref_translate = {1,0,-map_ref_points_[0],0,1,map_ref_points_[1],0,0,1};

    transform *= proj_ref_translate;
    boost::qvm::rotate_z(transform,-grivation_);
    transform *= scale_mat;
    transform *= map_ref_translate;

    boost::qvm::mat<double,2,2> transform_trimmed = {A<0,0>(transform),A<0,1>(transform),A<1,0>(transform),A<1,1>(transform)};
    if (to_projected_ != transform_trimmed)
    {
        to_projected_ = transform_trimmed;
    }
}

Spatial2D Georeferencing::ToProjectedCoords(Spatial2D coord) const
{
    boost::qvm::vec<float,2> src = {coord[0],coord[1]};
    boost::qvm::vec<float,2> dst = to_projected_ * src;
    return Spatial2D({X(dst),Y(dst)});
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

void Georeferencing::SetProjectedRefPoints(Spatial2D coords)
{
    projected_ref_points_ = coords;
}

void Georeferencing::SetMapRefPoints(Spatial2D coord)
{
    map_ref_points_ = coord;
}
