//
// Created by ace on 2026-01-22.
//

#include <foliage/json_builder.h>

#include "loguru.hpp"
#include "foliage/seed.h"


Json::Value JsonBuilder::FromGeneratedData(const Seed& data)
{
    Json::Value result = Json::objectValue;
    result["x"] = data.coordinate[0];
    result["y"] = data.coordinate[1];
    result["scale"] = data.scale;
    result["id"] = data.species_id;

    return result;
}

Json::Value JsonBuilder::FromGeneratedDataVec(const std::vector<Seed>& datas)
{
    Json::Value result = Json::arrayValue;
    for (const auto& data : datas)
    {
        result.append(FromGeneratedData(data));
    }

    return result;
}

Json::Value JsonBuilder::FromMapData(const MapData data)
{
    Json::Value result = Json::objectValue;
    result["width"] = data.width;
    result["height"] = data.height;
    result["h_offset"] = data.h_offset;
    result["v_offset"] = data.v_offset;
    return result;
}

Json::Value JsonBuilder::FromConfig(const OutputConfig output)
{
    Json::Value result = Json::objectValue;
    result["instances"] = output.instances.string();
    result["height_map"] = output.height_map.string();
    result["map_data"] = output.map_data.string();
    result["species"] = output.species.string();
    result["map_texture"] = output.map_texture.string();
    return result;
}
