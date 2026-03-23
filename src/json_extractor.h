//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_JSONEXTRACTOR_H
#define GENERATOR_JSONEXTRACTOR_H
#include <json/value.h>

#include <foliage/species_attribute.h>


class JsonExtractor
{
    using Value = Json::Value;
public:
    static std::vector<SpeciesAttribute> Extract(Value root);

private:
    static Gaussian ParseGaussian(Value root);
    static Range ParseRange(Value root);

};


#endif //GENERATOR_JSONEXTRACTOR_H