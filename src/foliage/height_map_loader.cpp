//
// Created by ace on 2026-02-26.
//
#include "foliage/height_map_loader.h"
#include <filesystem>

HeightMap HeightMapLoader::LoadHeightMap(std::filesystem::path path)
{
    CImg img(path.c_str());
    // TODO: Add scaling
    return HeightMap(std::move(img));
}

