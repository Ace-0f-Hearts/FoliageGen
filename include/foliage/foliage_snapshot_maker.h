//
// Created by ace on 2026-03-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H
#define PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H
#include "foliage_map.h"
#include "seed.h"
#include <glm/vec3.hpp>

#include "spatial/bounding_box.h"


class FoliageSnapshotMaker
{
public:
    static FoliageMap CreateSnapshot(std::vector<Seed>& seeds,BoundingBox2D bbox, size_t number_of_species);
private:
    static glm::vec3 GetColorValue(size_t point, size_t number_of_species);
};

#endif //PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H