//
// Created by ace on 2026-03-20.
//

#include <loguru.hpp>
#include <foliage/foliage_snapshot_maker.h>

#include "spatial/bounding_box.h"


void FoliageSnapshotMaker::CreateSnapshot(FoliageMap& map, std::vector<Seed>& seeds, BoundingBox2D bbox, size_t number_of_species)
{
    int x_offset, y_offset;

    x_offset = std::abs(std::floor(bbox.min()[0]));
    y_offset = std::abs(std::floor(bbox.min()[1]));

    int c = 0;
    for (auto &seed: seeds)
    {


        glm::vec3 value;

        if (seed.IsActive())
        {
            if (seed.IsClassifed())
            {
                value = GetColorValue(seed.id + 1, number_of_species);
            }
            else
            {
                value = {0.4f,0.4f,0.4f};
            }
        }
        else
        {
            value = {0.0f, 0.0f, 0.0f};
        }

        int x, y;
        x = std::round(seed.coordinate[0]) + x_offset;
        y = std::round(seed.coordinate[1]) + y_offset;

        map.map()(x,y,0,0) = value.x;
        map.map()(x,y,0,1) = value.y;
        map.map()(x,y,0,2) = value.z;
    }


}

void FoliageSnapshotMaker::CreateMapMask(FoliageMap& map, std::vector<std::unique_ptr<Object>>& areas, BoundingBox2D bbox)
{
    int x_offset, y_offset;

    x_offset = std::abs(std::floor(bbox.min()[0]));
    y_offset = std::abs(std::floor(bbox.min()[1]));

    int c = 0;

    for (const auto& obj: areas)
    {

        size_t width = std::ceil(obj->bounding_box().width());
        size_t height = std::ceil(obj->bounding_box().height());

        for (size_t i = 0; i < width; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                Spatial::Spatial2D pix({static_cast<float>(i),static_cast<float>(j)});

                int x,y;
                x = i + x_offset + std::round(obj->bounding_box().min()[0]);
                y = j + y_offset + std::round(obj->bounding_box().min()[1]);

                pix += obj->bounding_box().min();
                if (obj->IsIntersecting(pix))
                {
                    map.map()(x,y,0,0) = 0.f;
                    map.map()(x,y,0,1) = 0.f;
                    map.map()(x,y,0,2) = 0.f;

                }
            }
        }
    }
}

void FoliageSnapshotMaker::CreateSnapshot(FoliageMap& map, std::vector<Object*> areas, BoundingBox2D bbox)
{
    int x_offset, y_offset;

    x_offset = std::abs(std::floor(bbox.min()[0]));
    y_offset = std::abs(std::floor(bbox.min()[1]));

    int c = 0;

    for (const auto& obj: areas)
    {

        size_t width = std::ceil(obj->bounding_box().width());
        size_t height = std::ceil(obj->bounding_box().height());

        for (size_t i = 0; i < width; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                Spatial::Spatial2D pix({static_cast<float>(i),static_cast<float>(j)});

                int x,y;
                x = i + x_offset + std::round(obj->bounding_box().min()[0]);
                y = j + y_offset + std::round(obj->bounding_box().min()[1]);

                pix += obj->bounding_box().min();
                if (obj->IsIntersecting(pix))
                {
                    if (obj->type() == AreaO)
                    {
                        if (obj->symbol()->IsObstructing())
                        {
                            map.map()(x,y,0,0) -= 64.f;
                        } else
                        {
                            map.map()(x,y,0,2) -= 64.f;
                        }
                    } else if (obj->type() == PathO)
                    {
                        if (obj->symbol()->IsObstructing())
                        {
                            map.map()(x,y,0,1) -= 64.f;
                        }
                    }
                }
            }
        }
    }
}

glm::vec3 FoliageSnapshotMaker::GetColorValue(size_t point, size_t number_of_species)
{
    glm::vec3 value;
    float ratio = static_cast<float>(point) / static_cast<float>(number_of_species);
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

