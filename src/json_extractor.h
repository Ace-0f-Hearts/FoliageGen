//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONEXTRACTOR_H
#define GENERATOR_JSONEXTRACTOR_H
#include <json/value.h>

#include <foliage/species_attribute.h>

#include "foliage/diffusion_zone.h"
#include "orienteering/symbol_attribute.h"


class JsonExtractor
{
    using Value = Json::Value;
public:
    static std::vector<SpeciesAttribute> ExtractSpeciesAttributes(Value& root);
    static std::vector<SymbolAttribute> ExtractSymbolAttributes(Value& root);
    static std::vector<DiffusionZone> ExtractDiffusionZones(Value& root);
private:
    static Gaussian ParseGaussian(Value& root);
    static Range ParseRange(Value& root);
    static SymbolAttribute ParseSymbolAttribute(Value& root);
    static DiffusionZone ParseDiffusionZone(Value& root);
};


#endif //GENERATOR_JSONEXTRACTOR_H