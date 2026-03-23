//
// Created by ace on 2026-03-20.
//

#include <loguru.hpp>
#include <foliage/foliage_snapshot_maker.h>

#include "spatial/bounding_box.h"


FoliageMap FoliageSnapshotMaker::CreateSnapshot(std::vector<Seed>& seeds, BoundingBox2D bbox, size_t number_of_species)
{
    int x_offset, y_offset;

    x_offset = std::abs(std::floor(bbox.min()[0]));
    y_offset = std::abs(std::floor(bbox.min()[1]));

    int c = 0;
    CImg<float> f_map(bbox.width(),bbox.height(),1,3,1000.f);
    LOG_S(INFO) << bbox.width() << "\t" << bbox.height();
    for (auto &seed: seeds)
    {


        glm::vec3 value;

        if (seed.IsActive())
        {
            value = GetColorValue(seed.id, number_of_species);
            // value = { 1.f, 1.f, 1.f};
        }
        else
        {
            value = {0.0f, 0.0f, 0.0f};
        }

        int x, y;
        x = std::round(seed.coordinate[0]) + x_offset;
        y = std::round(seed.coordinate[1]) + y_offset;

        f_map(x,y,0,0) = value.x;
        f_map(x,y,0,1) = value.y;
        f_map(x,y,0,2) = value.z;

    }

    return f_map;
}

glm::vec3 FoliageSnapshotMaker::GetColorValue(size_t point, size_t number_of_species)
{
    glm::vec3 value;
    float ratio = point / number_of_species;

    value.x = 256.f * ratio;
    if (point % 2 == 0)
    {
        value.y = 256.f * .6f * ratio;
    }
    else
    {
        value.y = 256.f - 256.f * 0.8f * ratio;
    }

    value.z = 256.f * ratio - value.x * ratio;

    return value;
}

