//
// Created by ace on 2026-01-28.
//
#include <algorithm>
#include <ranges>
#include <orienteering/map.h>
#include <orienteering/object.h>
#include <orienteering/symbol.h>

#include "../../apps/app.h"
#include "orienteering/path_object.h"

void Map::AppendSymbol(std::unique_ptr<Symbol> symbol)
{
    symbols_.push_back(std::move(symbol));
}

void Map::AppendObject(std::unique_ptr<Object> obj)
{
    objects_.push_back(std::move(obj));
}

int Orienteering::Map::GetSymbolOfTypeAmount()
{
    return -1; //TODO: Implement
}

int Orienteering::Map::GetSymbolAmount()
{
    return symbols_.size();
}

int Orienteering::Map::GetObjectAmount()
{
    return objects_.size();
}

int Orienteering::Map::GetObjectOfSymbolAmount()
{
    return -1; //TODO: Implement
}

Orienteering::Symbol& Orienteering::Map::GetSymbolById(int id)
{
}

Orienteering::Symbol& Orienteering::Map::GetObjectById(int id)
{
}

void Orienteering::Map::ClearSymbols()
{
    symbols_.clear();
}

void Orienteering::Map::ClearObjects()
{
    objects_.clear();
}

void Orienteering::Map::Clear()
{
    ClearObjects();
    ClearSymbols();
}

void Orienteering::Map::RemoveObjectsOfSymbol(Symbol& symbol)
{
    objects_.erase(std::ranges::remove_if(objects_,[&symbol](const auto& o)->bool{
        return o->HasSymbolOf(&symbol);
    }).begin(),objects_.end());
}

void Orienteering::Map::RemoveSymbol(Symbol& symbol)
{
    RemoveObjectsOfSymbol(symbol);
    symbols_.erase(std::ranges::remove_if(symbols_,[&symbol](const auto& s) -> bool {return *s == symbol;}).begin(), symbols_.end());
}
