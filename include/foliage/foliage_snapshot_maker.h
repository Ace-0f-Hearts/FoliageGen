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

    FoliageSnapshotMaker(float resolution_mult, const BoundingBox2D& bbox, float default_value = 500);

    void RasterizeSeeds(std::vector<Seed>& seeds, const BoundingBox2D& bbox, size_t number_of_species);
    void RasterizeObjects(std::vector<Object*> areas, BoundingBox2D bbox, bool write_id = false, bool write_cmyk = false);

    FoliageMap& GetMap();
private:
    glm::vec3 GetColorValue(size_t point, size_t number_of_species);

    FoliageMap f_map_;
    const float kResolution_mult;
};

#endif //PROCEDURALFOLIAGEGENERATOR_FOLIAGE_SNAPSHOT_MAKER_H