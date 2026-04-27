//
// Created by ace on 2026-04-21.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_CONFIG_H
#define PROCEDURALFOLIAGEGENERATOR_CONFIG_H
#include <filesystem>

struct MapData
{
    float width;
    float height;
    float h_offset;
    float v_offset;
};



struct OutputConfig
{
    std::filesystem::path instances;
    std::filesystem::path species;
    std::filesystem::path map_data;
    std::filesystem::path height_map;
    std::filesystem::path map_texture;
};

#endif //PROCEDURALFOLIAGEGENERATOR_CONFIG_H