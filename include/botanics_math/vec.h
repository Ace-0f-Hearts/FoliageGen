//
// Created by ace on 2026-02-25.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_VEC_H
#define PROCEDURALFOLIAGEGENERATOR_VEC_H
#include <iosfwd>
#include <ostream>


struct Vec2
{
    float x;
    float y;

    Vec2(float x,float y):x(x),y(y){}

    Vec2 operator+(Vec2 const& rhs) const;
    Vec2 operator-(Vec2 const& rhs) const;
    Vec2 operator*(float n) const;
    Vec2 operator/(float n) const;
    bool operator==(Vec2 const& rhs) const;
};

struct Vec3
{
    float x;
    float y;
    float z;

    Vec3(float x,float y,float z):x(x),y(y),z(z){};

    Vec3 operator+(Vec3 const& rhs) const;
    Vec3 operator-(Vec3 const& rhs) const;
    Vec3 operator*(float n) const;
    Vec3 operator/(float n) const;
    bool operator==(Vec3 const& rhs) const;
};

Vec2 Normalize(Vec2 const& v);
Vec3 Normalize(Vec3 const& v);

Vec3 Cross(Vec3 const& a,Vec3 const& b);

float Dot(Vec3 const& a, Vec3 const& b);

float Normal(Vec2 const& v);
float Normal(Vec3 const& v);

inline std::ostream& operator<<(std::ostream& os, Vec2 const& v)
{
    return os << v.x << ' ' << v.y;
}
inline std::ostream& operator<<(std::ostream& os, Vec3 const& v)
{
    return os << v.x << ' ' << v.y << ' ' << v.z;
}

static const auto kUp = Vec3(0.f,1.f,0.f);

#endif //PROCEDURALFOLIAGEGENERATOR_VEC_H
