//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONEXTRACTOR_H
#define GENERATOR_JSONEXTRACTOR_H
#include <json/value.h>

#include "SpeciesAttribute.h"


class JsonExtractor
{
    using Value = Json::Value;
public:
    JsonExtractor();
    ~JsonExtractor();

    std::vector<SpeciesAttribute> Extract(Value root,char* err = nullptr);
private:

};


#endif //GENERATOR_JSONEXTRACTOR_H