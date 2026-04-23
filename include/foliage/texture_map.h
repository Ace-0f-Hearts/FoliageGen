//
// Created by ace on 2026-02-26.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_TEXTURE_H
#define PROCEDURALFOLIAGEGENERATOR_TEXTURE_H

#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0
#include <CImg.h>

#include "botanics_math/coord.h"
#include "botanics_math/dim.h"

using namespace cimg_library;

class TextureMap
{
public:
    TextureMap() = delete;
    TextureMap(CImg<> const& map);
    ~TextureMap() = default;
    [[nodiscard]] Dim2 Dim() const;
    [[nodiscard]] int Width() const;
    [[nodiscard]] int Height() const;
    [[nodiscard]] const CImg<>& map() const;
    [[nodiscard]] CImg<>& map();
protected:
    [[nodiscard]] float At_(Coord2 const& coord) const;
    void At_(Coord2 const& coord, float value);

private:
    CImg<> map_;
};

#endif //PROCEDURALFOLIAGEGENERATOR_TEXTURE_H