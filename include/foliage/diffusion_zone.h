//
// Created by ace on 2026-03-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H
#define PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H
#include "spatial/spatial_coordinates.h"

struct DiffusionZone
{
  DiffusionZone(Spatial::Spatial2D center, float radius, unsigned id) : center(center), radius(radius), id(id) {};

  Spatial::Spatial2D center;
  float radius;
  unsigned int id;
};

inline std::ostream& operator<<(std::ostream& os, const DiffusionZone& diffusion_zone)
{
  os << "(" << diffusion_zone.center << "|" << diffusion_zone.radius << "|" << diffusion_zone.id << ")";
  return os;
}

#endif //PROCEDURALFOLIAGEGENERATOR_DIFFUSION_ZONE_H