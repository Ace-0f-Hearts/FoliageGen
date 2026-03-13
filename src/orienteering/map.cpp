//
// Created by ace on 2026-01-28.
//
#include <algorithm>
#include <cassert>
#include <ranges>
#include <orienteering/map.h>
#include <orienteering/object.h>
#include <orienteering/symbol.h>

#include "../../apps/app.h"
#include "orienteering/path_object.h"

void Map::AppendSymbol(std::unique_ptr<Symbol> symbol)
{
    assert(symbol.get());
    symbols_.push_back(std::move(symbol));
}

void Map::AppendObject(std::unique_ptr<Object> obj)
{
    assert(obj.get());
    objects_.push_back(std::move(obj));
}

size_t Orienteering::Map::GetSymbolOfTypeAmount(SymbolType type) const
{
    auto predicate = [type](const std::unique_ptr<Symbol>& symbol) -> bool {return symbol->flags() & type;};

    return std::ranges::count_if(symbols_,predicate);
}

size_t Map::GetObjectOfTypeAmount(ObjectType type) const
{
    auto predicate = [type](const std::unique_ptr<Object>& object) -> bool
    {
        assert(object.get());
        return object->type() == type;
    };

    return std::ranges::count_if(objects_,predicate);
}

size_t Orienteering::Map::GetSymbolAmount() const
{
    return symbols_.size();
}

size_t Orienteering::Map::GetObjectAmount() const
{
    return objects_.size();
}

size_t Orienteering::Map::GetObjectOfSymbolAmount(Symbol* symbol) const
{
    auto predicate = [symbol](const std::unique_ptr<Object>& object) -> bool {return object->symbol() == symbol;};

    return std::ranges::count_if(objects_,predicate);
}

Symbol* Orienteering::Map::GetSymbolById(size_t id)
{
    auto predicate = [id](const std::unique_ptr<Symbol>& symbol) -> bool {return symbol->id() == id;};
    auto symbol = std::ranges::find_if(symbols_,predicate);

    if (symbol != symbols_.end())
    {
        return symbol->get();
    }
    return nullptr;
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
