//
// Created by ace on 2026-03-20.
//

#include <loguru.hpp>
#include <foliage/foliage_snapshot_maker.h>

#include "spatial/bounding_box.h"


void FoliageSnapshotMaker::RasterizeSeeds(const std::vector<Seed>& seeds, const BoundingBox2D& bbox, size_t number_of_species)
{
    int x_offset, y_offset;

    x_offset = -(std::floor(bbox.min()[0] * kResolution_mult));
    y_offset = -(std::floor(bbox.min()[1] * kResolution_mult));

    for (const auto &seed: seeds)
    {

        unsigned char r;
        unsigned char g;
        unsigned char b;
        if (seed.IsActive())
        {
            float ratio = static_cast<float>(seed.species_id + 1) / static_cast<float>(number_of_species);
            r = std::round(256.f * ratio);
            g = std::round(256.f - 256.f * ratio);
            b = std::round(sin(ratio + (seed.species_id + 1) * M_PI) * 256.f);
        }
        else
        {
            r = 0; g = 0; b = 0;
        }

        int x, y;
        x = std::clamp(static_cast<int>(std::round(seed.coordinate[0] * kResolution_mult) + x_offset),0,f_map_.map().width() - 1);
        y = std::clamp(static_cast<int>(std::round(seed.coordinate[1] * kResolution_mult) + y_offset),0,f_map_.map().height() - 1);



        const unsigned char color[] = {r,g,b};
        f_map_.map().draw_circle(x,y,seed.scale / 2.f,color);
    }


}


void FoliageSnapshotMaker::RasterizeObjects(const std::vector<Object*>& objects, const BoundingBox2D& bbox, bool write_id, bool write_cmyk)
{
    int x_offset, y_offset;

    x_offset = std::floor(bbox.min()[0] * kResolution_mult);
    y_offset = std::floor(bbox.min()[1] * kResolution_mult);
    unsigned char disallow[] = {255,0,0};
    unsigned char allow[] = {0,255,0};

    for (const auto& obj: objects)
    {
        // if (obj->symbol()->type() == PointS)
        // {
        //     auto center = obj->GetPoints().front();
        //
        //     int x,y;
        //     x = std::clamp(static_cast<int>(x_offset + center[0] * kResolution_mult),0,f_map_.map().width()-1);
        //     y = std::clamp(static_cast<int>(y_offset + center[1] * kResolution_mult),0,f_map_.map().height()-1);
        //
        //     if (obj->symbol()->IsObstructing())
        //     {
        //         f_map_.map().draw_circle(x,y,obj->symbol()->GetRadiusOfInfluence(),disallow);
        //     } else
        //     {
        //         f_map_.map().draw_circle(x,y,obj->symbol()->GetRadiusOfInfluence(),allow);
        //     }
        //     continue;
        // }



        int width = std::ceil(obj->bounding_box().width() * kResolution_mult);
        int height = std::ceil(obj->bounding_box().height() * kResolution_mult);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Spatial::Spatial2D pix({static_cast<float>(i / kResolution_mult),static_cast<float>(j / kResolution_mult)});

                int x = std::clamp(static_cast<int>(i - x_offset + std::round(obj->bounding_box().min()[0] * kResolution_mult)), 0,
                                   f_map_.map().width() - 1);
                int y = std::clamp(static_cast<int>(j - y_offset + std::round(obj->bounding_box().min()[1] * kResolution_mult)), 0,
                                   f_map_.map().height() - 1);


                pix += obj->bounding_box().min();
                if (obj->IsIntersecting(pix))
                {

                    if (write_id)
                    {
                        // Store the ID of the object in the pixel
                        f_map_.map()(x,y,0,0) = obj->symbol()->GetId();
                        f_map_.map()(x,y,0,1) = obj->symbol()->GetId();
                        f_map_.map()(x,y,0,2) = obj->symbol()->GetId();
                        continue;
                    }

                    if (obj->type() == AreaO)
                    {
                        if (obj->symbol()->IsObstructing())
                        {
                            f_map_.map()(x,y,0,0) -= 64.f;
                        } else
                        {
                            f_map_.map()(x,y,0,2) -= 64.f;
                        }
                    } else if (obj->type() == PathO)
                    {
                        if (obj->symbol()->IsObstructing())
                        {
                            f_map_.map()(x,y,0,1) -= 64.f;
                        }
                    } else if (obj->type() == PointO)
                    {
                        f_map_.map()(x,y,0,1) -= 64.f;
                        f_map_.map()(x,y,0,2) -= 64.f;
                    }
                }
            }
        }
    }
}


void FoliageSnapshotMaker::RasterizeUnmarkedForests(const MapBoundaryCalculator& boundary, const BoundingBox2D& bbox)
{
    const int x_offset = -std::floor(bbox.min()[0] * kResolution_mult);
    const int y_offset = -std::floor(bbox.min()[1] * kResolution_mult);

    int width = std::ceil(bbox.width() * kResolution_mult);
    int height = std::ceil(bbox.height() * kResolution_mult);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Spatial::Spatial2D pix({static_cast<float>(i / kResolution_mult),static_cast<float>(j / kResolution_mult)});

                int x,y;
                x = std::clamp(static_cast<int>(i + x_offset + std::round(bbox.min()[0] * kResolution_mult)),0,f_map_.map().width()-1);
                y = std::clamp(static_cast<int>(j + y_offset + std::round(bbox.min()[1] * kResolution_mult)),0,f_map_.map().height()-1);

                pix += bbox.min();
                auto is_inside_map = boundary.IsCoordinateInsideMap(pix);


                if (is_inside_map && f_map_.map()(x,y,0,0) == 0 && f_map_.map()(x,y,0,1) == 0 && f_map_.map()(x,y,0,2) == 0)
                {

                    f_map_.map()(x,y,0,0) = -128.f;
                    f_map_.map()(x,y,0,1) = -128.f;
                    f_map_.map()(x,y,0,2) = -128.f;
                }
            }
        }
}

FoliageSnapshotMaker::FoliageSnapshotMaker(float resolution_mult, const BoundingBox2D& bbox, float default_value) :
f_map_(CImg<>(std::round(bbox.width() * resolution_mult) ,std::round(bbox.height() * resolution_mult),1,3,default_value)),
kResolution_mult(resolution_mult)
{

}

FoliageMap& FoliageSnapshotMaker::GetMap()
{
    return f_map_;
}
