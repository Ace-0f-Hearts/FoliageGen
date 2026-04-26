//
// Created by ace on 2026-02-13.
//

#include <spatial/spatial_coordinate.h>


float Spatial::Dot(Spatial2D const& a, Spatial2D const& b)
{
    return a[0] * b[0] + a[1] * b[1];
}
float Spatial::Dot(Spatial3D const& a, Spatial3D const& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

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

template <unsigned int N>
float Spatial::Length(SpatialCoordinate<N> const& a)
{
    auto result = 0.f;
    for (unsigned int i = 0; i < N; ++i)
    {
        result += a[i] * a[i];
    }
    return std::sqrt(result);
}

template <unsigned int N>
float Spatial::Dot(SpatialCoordinate<N> const& a, SpatialCoordinate<N> const& b)
{
    auto result = 0.f;
    for (unsigned int i = 0; i < N; ++i)
    {
        result += a[i] * b[i];
    }
    return result;
}

template <unsigned int N>
Spatial::SpatialCoordinate<N> Spatial::Normalize(SpatialCoordinate<N> const& a)
{
    auto length = Length(a);
    return a / length;
}

Spatial::Spatial2D Spatial::Cross(Spatial2D const& a, Spatial2D const& b)
{
    return Spatial2D{a[2] - b[2], b[1] - a[1]};
}

Spatial::Spatial2D Spatial::Normalize(Spatial2D const& a)
{
    auto length = Length(a);
    return a / length;
}

Spatial::Spatial3D Spatial::Normalize(Spatial3D const& a)
{
    auto length = Length(a);
    return a / length;
}
