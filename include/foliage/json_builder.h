//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONBUILDER_H
#define GENERATOR_JSONBUILDER_H
#include <json/value.h>

#include "seed.h"


class JsonBuilder
{

public:
    static Json::Value FromGeneratedData(const Seed& data);
    static Json::Value FromGeneratedDataVec(const std::vector<Seed>& datas);

};


#endif //GENERATOR_JSONBUILDER_H