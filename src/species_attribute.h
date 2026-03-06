//
// Created by ace on 2026-01-22.
//

#ifndef GENERATOR_SPECIESATTRIBUTE_H
#define GENERATOR_SPECIESATTRIBUTE_H
#include <string>
#include <utility>

#include <botanics_math/gaussian.h>
#include <botanics_math/range.h>

struct SpeciesAttribute
{
    using string = std::string;

    SpeciesAttribute() = delete;
    SpeciesAttribute(string  name, const Range growth, const Gaussian slope , const Gaussian elevation)
        : name(std::move(name)), growth(growth), slope(slope), elevation(elevation)
    {}

    string name;
    Range growth;
    Gaussian slope;
    Gaussian elevation;
};


#endif //GENERATOR_SPECIESATTRIBUTE_H