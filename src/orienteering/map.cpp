//
// Created by ace on 2026-01-28.
//
#include <algorithm>
#include <cassert>
#include <ranges>
#include <orienteering/map.h>
#include <orienteering/object.h>
#include <orienteering/symbol.h>


void OrienteeringMap::AppendSymbol(std::unique_ptr<Symbol> symbol)
{
    assert(symbol.get());
    symbols_.push_back(std::move(symbol));
}

void OrienteeringMap::AppendObject(std::unique_ptr<Object> obj)
{
    assert(obj.get());
    objects_.push_back(std::move(obj));
    UpdateBoundingBox();
}

size_t Orienteering::OrienteeringMap::GetSymbolOfTypeAmount(SymbolType type) const
{
    auto predicate = [type](const std::unique_ptr<Symbol>& symbol) -> bool {return symbol->flags() & type;};

    return std::ranges::count_if(symbols_,predicate);
}

size_t OrienteeringMap::GetObjectOfTypeAmount(ObjectType type) const
{
    auto predicate = [type](const std::unique_ptr<Object>& object) -> bool
    {
        assert(object.get());
        return object->type() == type;
    };

    return std::ranges::count_if(objects_,predicate);
}

size_t Orienteering::OrienteeringMap::GetSymbolAmount() const
{
    return symbols_.size();
}

size_t Orienteering::OrienteeringMap::GetObjectAmount() const
{
    return objects_.size();
}

size_t Orienteering::OrienteeringMap::GetObjectOfSymbolAmount(Symbol* symbol) const
{
    auto predicate = [symbol](const std::unique_ptr<Object>& object) -> bool {return object->symbol() == symbol;};

    return std::ranges::count_if(objects_,predicate);
}

Symbol* Orienteering::OrienteeringMap::GetSymbolById(size_t id)
{
    auto predicate = [id](const std::unique_ptr<Symbol>& symbol) -> bool {return symbol->id() == id;};
    auto symbol = std::ranges::find_if(symbols_,predicate);

    if (symbol != symbols_.end())
    {
        return symbol->get();
    }
    return nullptr;
}

BoundingBox2D OrienteeringMap::GetBoundingBox() const
{
    return bounding_box_;
}

std::vector<std::unique_ptr<Symbol>>& OrienteeringMap::GetSymbols()
{
    return symbols_;
}

std::vector<std::unique_ptr<Object>>& OrienteeringMap::GetObjects()
{
    return objects_;
}

void Orienteering::OrienteeringMap::ClearSymbols()
{
    symbols_.clear();
}

void Orienteering::OrienteeringMap::ClearObjects()
{
    objects_.clear();
}

void Orienteering::OrienteeringMap::Clear()
{
    ClearObjects();
    ClearSymbols();
}

void Orienteering::OrienteeringMap::RemoveObjectsOfSymbol(Symbol& symbol)
{
    objects_.erase(std::ranges::remove_if(objects_,[&symbol](const auto& o)->bool{
        return o->HasSymbolOf(&symbol);
    }).begin(),objects_.end());
}

void Orienteering::OrienteeringMap::RemoveSymbol(Symbol& symbol)
{
    RemoveObjectsOfSymbol(symbol);
    symbols_.erase(std::ranges::remove_if(symbols_,[&symbol](const auto& s) -> bool {return *s == symbol;}).begin(), symbols_.end());
}

void OrienteeringMap::UpdateBoundingBox()
{
    Spatial::Spatial2D min, max;
    min = bounding_box_.min();
    max = bounding_box_.max();
    for (auto& object: objects_)
    {
        auto bbox = object->bounding_box();

        if (min[0] > bbox.min()[0])
        {
            min[0] = bbox.min()[0];
        }

        if (min[1] > bbox.min()[1])
        {
            min[1] = bbox.min()[1];
        }

        if (max[0] < bbox.max()[0])
        {
            max[0] = bbox.max()[0];
        }

        if (max[1] < bbox.max()[1])
        {
            max[1] = bbox.max()[1];
        }

    }

    bounding_box_ = {min,max};
}

void OrienteeringMap::AppendColor(MapColor color)
{
    colors_.push_back(color);
}

size_t OrienteeringMap::GetColorsAmount() const
{
    return colors_.size();
}

std::vector<MapColor>& OrienteeringMap::GetColors()
{
    return colors_;
}

std::vector<Object*> OrienteeringMap::GetObjectsOfType(ObjectType type) const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (object->type() == AreaO)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

std::vector<Object*> OrienteeringMap::GetObstructingAreas() const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (object->symbol()->IsObstructing() && object->type() == AreaO)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

std::vector<Object*> OrienteeringMap::GetFreeAreas() const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (!object->symbol()->IsObstructing() && object->type() == AreaO)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}
