//
// Created by ace on 2026-01-28.
//
#include "../../include/orienteering/map.h"

void Orienteering::Map::AddSymbol(Symbol* symbol)
{
    symbols_.push_back(symbol);
}

void Orienteering::Map::AddObject(Object* obj)
{
    objects_.push_back(obj);
}