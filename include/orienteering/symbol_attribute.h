//
// Created by ace on 2026-03-19.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_SYMBOL_ATTRIBUTE_H
#define PROCEDURALFOLIAGEGENERATOR_SYMBOL_ATTRIBUTE_H
#include <cstdint>

struct SymbolAttribute
{
    uint32_t id;
    uint8_t flags;
    float radius_of_influence;
};

#endif //PROCEDURALFOLIAGEGENERATOR_SYMBOL_ATTRIBUTE_H