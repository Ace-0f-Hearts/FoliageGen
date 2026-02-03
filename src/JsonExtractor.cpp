//
// Created by ace on 2026-01-22.
//

#include "JsonExtractor.h"

JsonExtractor::JsonExtractor()
{
}

JsonExtractor::~JsonExtractor()
{
}

std::vector<SpeciesAttribute> JsonExtractor::Extract(Value root,char* err)
{
    std::vector<SpeciesAttribute> attributes;
    for (auto item: root)
    {
        SpeciesAttribute attr{
            item["name"].asString(),
            item["growthRadius"].asFloat(),
            item["minElevation"].asFloat(),
            item["maxElevation"].asFloat()
        };
        attributes.push_back(attr);
    }

    return attributes;
}

