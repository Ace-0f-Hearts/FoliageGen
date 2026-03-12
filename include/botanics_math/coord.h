//
// Created by ace on 2026-02-26.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_COORD_H
#define PROCEDURALFOLIAGEGENERATOR_COORD_H

#include <fstream>
struct Coord2
{
    int x;
    int y;
};

inline Coord2 operator+(Coord2 const& a, Coord2 const& b)
{
    return Coord2{a.x + b.x, a.y + b.y};
}

inline Coord2 operator-(Coord2 const& a, Coord2 const& b)
{
    return Coord2{a.x - b.x, a.y - b.y};
}

inline std::ostream& operator<< (std::ostream& os, Coord2 const& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

#endif //PROCEDURALFOLIAGEGENERATOR_COORD_H