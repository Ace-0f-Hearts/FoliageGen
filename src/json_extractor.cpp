//
// Created by ace on 2026-01-22.
//

#include "json_extractor.h"

#include "../include/utility/not_implemented_error.h"



std::vector<SpeciesAttribute> JsonExtractor::Extract(Value root)
{
    std::vector<SpeciesAttribute> attributes;
    for (auto item: root)
    {

        const auto name = item["name"].asString();
        const auto growth_radius = ParseRange(item["growth_radius"]);
        const auto slope = ParseGaussian(item["slope"]);
        const auto elevation = ParseGaussian(item["elevation"]);

        SpeciesAttribute attr{name,growth_radius,slope,elevation};
        attributes.push_back(attr);
    }

    return attributes;
}

Gaussian JsonExtractor::ParseGaussian(Value root)
{
    float peak, avg, deviation;

    if (root.isArray())
    {
        peak = root[0].asFloat();
        avg = root[1].asFloat();
        deviation = root[2].asFloat();
    }
    else if (root.isObject())
    {
        peak = root["peak"].asFloat();
        avg = root["avg"].asFloat();
        deviation = root["deviation"].asFloat();
    }
    else
    {
        throw std::runtime_error("Couldn't parse Gaussian value. Item was instead: " + root.asString());
    }

    return Gaussian{peak,avg,deviation};
}

Range JsonExtractor::ParseRange(Value root)
{

    float min, max;
    if (root.isArray())
    {
        min = root[0].asFloat();
        max = root[1].asFloat();
    }
    else if (root.isObject())
    {
        min = root["min"].asFloat();
        max = root["max"].asFloat();
    }
    else
    {
        throw std::runtime_error("Couldn't parse Range value. Item was instead: " + root.asString());
    }

    return Range{min,max};
}

