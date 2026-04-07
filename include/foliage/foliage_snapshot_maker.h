//
// Created by ace on 2026-03-20.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H
#define PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H
#include "foliage_map.h"
#include "seed.h"
#include <glm/vec3.hpp>

#include "orienteering/path_object.h"
#include "spatial/bounding_box.h"
#include <memory>


class FoliageSnapshotMaker
{
public:

    FoliageSnapshotMaker(float resolution_mult, const BoundingBox2D& bbox);

    void CreateSnapshot(std::vector<Seed>& seeds,BoundingBox2D bbox, size_t number_of_species);
    void CreateSnapshot(std::vector<Object*> areas,BoundingBox2D bbox);
    void CreateMapMask(std::vector<std::unique_ptr<Object>>& areas,BoundingBox2D bbox);

    FoliageMap& GetMap();
private:
    glm::vec3 GetColorValue(size_t point, size_t number_of_species);

    FoliageMap f_map_;
    const float kResolution_mult;
};

#endif //PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H