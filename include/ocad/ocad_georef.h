//
// Created by ace on 2026-03-17.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_OCADGEOREFFIELDS_H
#define PROCEDURALFOLIAGEGENERATOR_OCADGEOREFFIELDS_H
#include "orienteering/georeferencing.h"

struct OcadGeoref
{
    double a { 0 };
    int m { 15000 }, x { 0 }, y { 0 }, i { 1000 }, r {0};

    void SetupGeoref(Georeferencing& georef) const;

};

inline std::ostream& operator<<(std::ostream& os, const OcadGeoref& georef)
{
    os
    << "(a: " << georef.a
    << " m: " << georef.m
    << " x: " << georef.x
    << " y: " << georef.y
    << " i: " << georef.i
    << " r: " << georef.r << ")";
    return os;
}

inline void OcadGeoref::SetupGeoref(Georeferencing& georef) const
{
    if (m > 0)
    {
        georef.SetScaleDenominator(m);
    }

    if (r)
    {

    }

    Spatial2D proj_ref_point{{static_cast<float>(x),static_cast<float>(y)}};
    georef.SetProjectedRefPoints(proj_ref_point);
    georef.SetCombinedScaleFactor(1.0);
    georef.SetGrivation(std::isfinite(a) ? a : 0);
}




#endif //PROCEDURALFOLIAGEGENERATOR_OCADGEOREFFIELDS_H
