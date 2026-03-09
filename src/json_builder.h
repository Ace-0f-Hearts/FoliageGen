//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONBUILDER_H
#define GENERATOR_JSONBUILDER_H
#include <json/value.h>

#include "generated_data.h"


class JsonBuilder
{
    using Value = Json::Value;
    using String = Json::String;
public:
    JsonBuilder();
    Value BuildNGet(const GeneratedData&);
    void Clear();
private:
    Value root_;
    String err_;
};


#endif //GENERATOR_JSONBUILDER_H