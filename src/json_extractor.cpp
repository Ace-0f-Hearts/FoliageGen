//
// Created by ace on 2026-01-22.
//

#include "json_extractor.h"

#include <loguru.hpp>

#include "../include/utility/not_implemented_error.h"



std::vector<SpeciesAttribute> JsonExtractor::ExtractSpeciesAttributes(Value& root)
{
    std::vector<SpeciesAttribute> attributes;
    for (auto item: root)
    {

        const auto id = item["id"].asUInt();
        const auto name = item["name"].asString();
        const auto growth_radius = ParseRange(item["growth_radius"]);
        const auto slope = ParseGaussian(item["slope"]);
        const auto elevation = ParseGaussian(item["elevation"]);

        SpeciesAttribute attr{id,name,growth_radius,slope,elevation};
        attributes.push_back(attr);
    }

    return attributes;
}

Gaussian JsonExtractor::ParseGaussian(Value& root)
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
        throw std::runtime_error("Couldn't parse Gaussian value. Item was instead: " + root.toStyledString());
    }

    return Gaussian{peak,avg,deviation};
}

Range JsonExtractor::ParseRange(Value& root)
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
        throw std::runtime_error("Couldn't parse Range value. Item was instead: " + root.toStyledString());
    }

    return Range{min,max};
}

std::vector<SymbolAttribute> JsonExtractor::ExtractSymbolAttributes(Value& root)
{
    std::vector<SymbolAttribute> attributes;
        LOG_S(INFO) << root.toStyledString();
    for (auto item: root)
    {
        attributes.push_back(ParseSymbolAttribute(item));
    }

    return attributes;
}

SymbolAttribute JsonExtractor::ParseSymbolAttribute(Value& root)
{
    uint32_t id;
    uint8_t flags;
    if (root.isObject())
    {
        id = root["id"].asUInt();
        flags = root["flags"].asUInt();
    }
    else
    {
        throw std::runtime_error("Couldn't parse SymbolAttribute. Item was instead: " + root.toStyledString());
    }
    return SymbolAttribute{id,flags};
}

std::vector<DiffusionZone> JsonExtractor::ExtractDiffusionZones(Value& root)
{
}

DiffusionZone JsonExtractor::ParseDiffusionZone(Value& root)
{
    float x, y, r;
    uint id;
    if (root.isObject())
    {
        id = root["id"].asUInt();
        x = root["x"].asFloat();
        y = root["y"].asFloat();
        r = root["r"].asFloat();
    }
    else
    {
        throw std::runtime_error("Couldn't parse DiffusionZone. Item was instead: " + root.toStyledString());
    }

    return DiffusionZone{{x,y},r,id};
}
