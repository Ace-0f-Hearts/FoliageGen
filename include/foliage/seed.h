//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SEED_H
#define PROCEDURALFOLIAGEGENERATOR_SEED_H
#include "spatial/spatial_coordinates.h"


enum SeedFlag
{
    Initial = 0x1,
    Active = 0x2,
    Classified = 0x4,
};

struct Seed
{
    Seed();
    Seed(Spatial::Spatial2D coord = {0,0},float scale = 1.f) : coordinate(coord), scale(scale), flags(0x0), id(0)
    {
    };

    bool IsActive() const { return flags & Active; };
    bool IsInitial() const {return flags & Initial; };
    bool IsClassifed() const { return flags & Classified; };

    Spatial::Spatial2D coordinate;
    float scale;
    uint8_t flags;
    uint32_t id;


};

#endif //PROCEDURALFOLIAGEGENERATOR_SEED_H