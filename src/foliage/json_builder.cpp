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
    int idx = 0;
    for (const auto& data : datas)
    {

        result.append(FromGeneratedData(data));
    }

    return result;
}


