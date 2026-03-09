//
// Created by ace on 2026-01-08.
//

#ifndef GENERATOR_PROCGEN_H
#define GENERATOR_PROCGEN_H
#include <spatial/bounding_box.h>
#include <spatial/poly_path.h>
#include <orienteering/object.h>

#include "spatial/path_collection.h"

class Generation
{
public:
    static std::vector<Spatial2D> InitializeSeeds(const BoundingBox2D& bounding_box, float density, Object& object);

private:
    static void Randomize(std::vector<Spatial2D>& seeds, float density, float factor = 1.f, float angle = 0);
    static std::vector<Spatial2D> Cull(std::vector<Spatial2D>& seeds,Spatial::PathCollection area);

};


#endif //GENERATOR_PROCGEN_H