//
// Created by ace on 2026-04-08.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_MASK_H
#define PROCEDURALFOLIAGEGENERATOR_MASK_H

#include <CImg.h>
#include <filesystem>
#include <optional>
#include "texture_map.h"
#include "orienteering/object.h"
#include "spatial/bounding_box.h"

using namespace cimg_library;

class Mask
{
public:
    Mask() = delete;
    Mask(BoundingBox2D bbox, float resolution_mult, int channels = 1);

    [[nodiscard]] int At(Coord2 const& coord, int channel = 0) const;
    void At(Coord2 const& coord, int value, int  channel = 0);
    Coord2 SpatialToMaskCoordinate(Spatial::Spatial2D const& coord) const;

    void Write(std::filesystem::path path);

    float resolution_mult() const;
    float x_offset() const;
    float y_offset() const;
private:
    BoundingBox2D bbox_;
    float resolution_mult_;
    CImg<int> img_;

    float x_offset_;
    float y_offset_;
};



class MaskMaker
{
public:
    MaskMaker();

    void CreateNewMask(BoundingBox2D bbox,float mask_resolution = 1.f, int channels = 1);
    void MaskObjects(std::vector<Object*> objects);
    void ClearMask();

    std::optional<Mask> GetMask();


private:
    float mask_resolution_{};
    BoundingBox2D bbox_;
    std::optional<Mask> mask_;
};


#endif //PROCEDURALFOLIAGEGENERATOR_MASK_H