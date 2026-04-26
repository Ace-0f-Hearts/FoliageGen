//
// Created by ace on 2026-04-08.
//
#include <cassert>
#include <foliage/mask.h>

#include "loguru.hpp"


Mask::Mask(BoundingBox2D bbox, float resolution_mult, int channels) :
    bbox_(bbox),
    resolution_mult_(resolution_mult),
    img_(static_cast<size_t>(std::ceil(bbox.width()) * resolution_mult),
         static_cast<size_t>(std::ceil(bbox.height()) * resolution_mult), 1, channels, 0),
    x_offset_(-std::floor(bbox.min()[0] * resolution_mult_)),
    y_offset_(-std::floor(bbox.min()[1] * resolution_mult_))
{
}

int Mask::At(Coord2 const& coord, int channel) const
{
    // LOG_S(INFO) << coord.x << " | " << coord.y << " | " << img_.width() << " | " << img_.height();
    // assert(coord.x >= 0);
    // assert(coord.y >= 0);
    // assert(coord.x < img_.width());
    // assert(coord.y < img_.height());
    int x = std::clamp(coord.x, 0, img_.width() - 1);
    int y = std::clamp(coord.y, 0, img_.width() - 1);
    return img_(coord.x, coord.y, 0, channel);
}

void Mask::At(Coord2 const& coord, int value, int channel)
{
    // LOG_S(INFO) << coord.x << " | " << coord.y << " | " << img_.width() << " | " << img_.height();
    // assert(coord.x >= 0);
    // assert(coord.y >= 0);
    // assert(coord.x < img_.width());
    // assert(coord.y < img_.height());
    int x = std::clamp(coord.x, 0, img_.width() - 1);
    int y = std::clamp(coord.y, 0, img_.width() - 1);
    img_(x, y, 0, channel) = value;
}

Coord2 Mask::SpatialToMaskCoordinate(Spatial::Spatial2D const& coord) const
{
    int x = static_cast<int>(std::floor(coord[0] * resolution_mult_ + x_offset_));
    int y = static_cast<int>(std::floor(coord[1] * resolution_mult_ + y_offset_));
    assert(x >= 0);
    assert(y >= 0);
    assert(x < img_.width());
    assert(y < img_.height());
    return {x, y};
}


MaskMaker::MaskMaker() = default;

void MaskMaker::CreateNewMask(BoundingBox2D bbox, float mask_resolution, int channels)
{
    mask_.emplace(bbox, mask_resolution, channels);
    mask_resolution_ = mask_resolution;
    bbox_ = bbox;
}

void MaskMaker::MaskObject(const Object* object)
{
    int x_offset = -static_cast<int>(std::floor(bbox_.min()[0] * mask_resolution_));
    int y_offset = -static_cast<int>(std::floor(bbox_.min()[1] * mask_resolution_));
    if (!mask_.has_value())
    {
        return;
    }

    size_t width = std::ceil(object->bounding_box().width() * mask_resolution_);
    size_t height = std::ceil(object->bounding_box().height() * mask_resolution_);

    float fi = 0.f;
    for (size_t i = 0; i < width; i++, fi += 1.0f / mask_resolution_)
    {
        float fj = 0.f;
        for (size_t j = 0; j < height; j++, fj += 1.0f / mask_resolution_)
        {
            int x, y;
            x = i + x_offset + std::round(object->bounding_box().min()[0] * mask_resolution_);
            y = j + y_offset + std::round(object->bounding_box().min()[1] * mask_resolution_);

            // Spatial::Spatial2D point({fi + obj->bounding_box().min()[0],fj + obj->bounding_box().min()[1]});
            Spatial::Spatial2D pix({
                static_cast<float>(i / mask_resolution_), static_cast<float>(j / mask_resolution_)
            });
            pix += object->bounding_box().min();
            if (object->IsIntersecting(pix))
            {
                // Store the ID of the object in the pixel
                mask_->At({x, y}, object->symbol()->GetId(), 0);
            }
        }
    }
}

void MaskMaker::MaskObjects(std::vector<Object*> objects)
{
    int x_offset = -static_cast<int>(std::floor(bbox_.min()[0] * mask_resolution_));
    int y_offset = -static_cast<int>(std::floor(bbox_.min()[1] * mask_resolution_));
    if (!mask_.has_value())
    {
        return;
    }
    for (const auto& obj : objects)
    {
        int width = std::ceil(obj->bounding_box().width() * mask_resolution_);
        int height = std::ceil(obj->bounding_box().height() * mask_resolution_);

        float fi = 0.f;
        for (int i = 0; i < width; i++, fi += 1.0f / mask_resolution_)
        {
            float fj = 0.f;
            for (int j = 0; j < height; j++, fj += 1.0f / mask_resolution_)
            {
                int x, y;
                x = std::clamp(i + x_offset + static_cast<int>(std::round(obj->bounding_box().min()[0] * mask_resolution_)),0,mask_->width() -1);
                y = std::clamp(j + y_offset + static_cast<int>(std::round(obj->bounding_box().min()[1] * mask_resolution_)),0, mask_->height() -1);

                Spatial::Spatial2D pix({
                    static_cast<float>(i / mask_resolution_), static_cast<float>(j / mask_resolution_)
                });
                pix += obj->bounding_box().min();
                if (obj->IsIntersecting(pix))
                {
                    // Store the ID of the object in the pixel
                    mask_->At({x, y}, obj->symbol()->GetId(), 0);
                }
            }
        }
    }
}

void MaskMaker::ClearMask()
{
    mask_.reset();
}

std::optional<Mask>& MaskMaker::GetMask()
{
    return mask_;
}

std::optional<Mask> MaskMaker::GetMask() const
{
    return mask_;
}

void Mask::Write(const std::filesystem::path& path) const
{
    auto img = img_.save(path.c_str());
}

float Mask::resolution_mult() const
{
    return resolution_mult_;
}

float Mask::x_offset() const
{
    return x_offset_;
}

float Mask::y_offset() const
{
    return y_offset_;
}
