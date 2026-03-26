//
// Created by ace on 2026-03-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H
#define PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H
#include "spatial/spatial_coordinates.h"

struct DiffusionZone
{
  Spatial::Spatial2D center_;
  float radius_;
  uint id;
};

#endif //PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H