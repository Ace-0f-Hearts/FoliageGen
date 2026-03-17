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

    static OcadGeoref FromGeoref(const Georeferencing& georef);
};

#endif //PROCEDURALFOLIAGEGENERATOR_OCADGEOREFFIELDS_H