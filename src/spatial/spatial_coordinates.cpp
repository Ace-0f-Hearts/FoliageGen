//
// Created by ace on 2026-02-13.
//

#include <spatial/spatial_coordinates.h>


float Spatial::Dot(Spatial2D const& a, Spatial2D const& b)
{
    return a[0] * b[0] + a[1] * b[1];
}
float Spatial::Dot(Spatial3D const& a, Spatial3D const& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Spatial2D Cross(Spatial2D const& a, Spatial2D const& b)
// {
//     return Spatial2D{a[2] * b[] - a.z * b.y,a.z*b.x - a.x * b.z,a.x * b.y - a.y * b.x};
// }

Spatial::Spatial3D Spatial::Cross(Spatial3D const& a, Spatial3D const& b)
{
    return Spatial3D{a[1] * b[2] - a[2] * b[1],a[2] * b[0] - a[0] * b[2],a[0] * b[1] - a[1] * b[0]};
}

float Spatial::Length(Spatial2D const& a)
{
    return std::sqrt(a[0] * a[0] + a[1] * a[1]);
}

float Spatial::Length(Spatial3D const& a)
{
    return std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}
