//
// Created by ace on 2026-02-25.
//
#include <cmath>
#include <iostream>
#include <botanics_math/vec.h>


Vec2 Vec2::operator+(Vec2 const& rhs) const
{
    return Vec2{x+rhs.x,y+rhs.y};
}

Vec2 Vec2::operator-(Vec2 const& rhs) const
{
    return Vec2{x-rhs.x,y-rhs.y};
}

Vec3 Vec3::operator+(Vec3 const& rhs) const
{
    return Vec3{x+rhs.x,y+rhs.y,z+rhs.z};
}

Vec3 Vec3::operator-(Vec3 const& rhs) const
{
    return Vec3{x-rhs.x,y-rhs.y,z-rhs.z};
}

Vec2 Vec2::operator*(float n) const
{
    return Vec2{x*n,y*n};
}

Vec2 Vec2::operator/(float n) const
{
    return Vec2{x/n,y/n};
}

Vec3 Vec3::operator*(float n) const
{
    return Vec3{x*n,y*n,z*n};
}

Vec3 Vec3::operator/(float n) const
{
    return Vec3{x/n,y/n,z/n};
}

Vec2 Normalize(Vec2 const& v)
{
    return v / Normal(v);
}

Vec3 Normalize(Vec3 const& v)
{
    return v / Normal(v);
}

Vec3 Cross(Vec3 const& a, Vec3 const& b)
{
    return Vec3{a.y * b.z - a.z * b.y,a.z*b.x - a.x * b.z,a.x * b.y - a.y * b.x};
}

float Dot(Vec3 const& a, Vec3 const& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Normal(Vec2 const& v)
{
    return std::sqrt(v.x*v.x+v.y*v.y);
}

float Normal(Vec3 const& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool Vec2::operator==(Vec2 const& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec3::operator==(Vec3 const& rhs) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}
