//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONBUILDER_H
#define GENERATOR_JSONBUILDER_H
#include <json/value.h>

#include "config.h"
#include "seed.h"


class JsonBuilder
{

public:
    static Json::Value FromGeneratedData(const Seed& data);
    static Json::Value FromGeneratedDataVec(const std::vector<Seed>& datas);

    static Json::Value FromMapData(const MapData data);
    static Json::Value FromConfig(const OutputConfig output);

};


#endif //GENERATOR_JSONBUILDER_H