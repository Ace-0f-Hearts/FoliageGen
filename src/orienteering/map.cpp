//
// Created by ace on 2026-01-28.
//
#include <algorithm>
#include <cassert>
#include <ranges>
#include <orienteering/map.h>
#include <orienteering/object.h>
#include <orienteering/symbol.h>


void OrienteeringMap::AppendSymbol(std::shared_ptr<Symbol> symbol)
{
    assert(symbol.get());

    auto id_to_search = symbol->GetId();
    auto predicate = [id_to_search](const std::shared_ptr<Symbol>& s){ return id_to_search == s->GetId(); };

    if (auto ptr = std::ranges::find_if(symbols_,predicate); ptr != symbols_.end())
    {
        // Combine the symbol with previously inserted one
        if (!ptr->operator->()->HasColor() && symbol->HasColor())
        {
            ptr->operator->()->SetColor(symbol->GetColor());
        }
    } else
    {
        symbols_.emplace_back(symbol);
    }

}

void OrienteeringMap::AppendObject(std::unique_ptr<Object> obj)
{
    assert(obj.get());
    objects_.push_back(std::move(obj));
    UpdateBoundingBox();
}

size_t Orienteering::OrienteeringMap::GetSymbolOfTypeAmount(SymbolType type) const
{
    auto predicate = [type](const std::shared_ptr<Symbol>& symbol) -> bool {return symbol->flags() & type;};

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
    auto predicate = [symbol](const std::unique_ptr<Object>& object) -> bool {return object->symbol()->GetId() == symbol->GetId();};

    return std::ranges::count_if(objects_,predicate);
}

Symbol* Orienteering::OrienteeringMap::GetSymbolById(size_t id)
{
    auto predicate = [id](const std::shared_ptr<Symbol>& symbol) -> bool {return symbol->GetId() == id;};
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

std::vector<std::shared_ptr<Symbol>>& OrienteeringMap::symbols()
{
    return symbols_;
}

std::vector<std::unique_ptr<Object>>& OrienteeringMap::objects()
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


    min = objects_.front()->bounding_box().min();
    max = objects_.front()->bounding_box().max();
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

std::vector<Object*> OrienteeringMap::GetIrrelevantFeatures() const
{
    std::vector<Object*> objects;
    for (auto& object: objects_)
    {
        if (object->symbol()->IsIrrelevant())
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

void OrienteeringMap::AppendColor(std::shared_ptr<MapColor> color)
{
    colors_.push_back(color);
}

size_t OrienteeringMap::GetColorsAmount() const
{
    return colors_.size();
}

std::vector<std::shared_ptr<MapColor>>& OrienteeringMap::colors()
{
    return colors_;
}

std::vector<Object*> OrienteeringMap::GetObjects() const
{
    std::vector<Object*> objects;
    for (auto& object: objects_)
    {
        objects.emplace_back(object.get());
    }

    return objects;
}

std::vector<Object*> OrienteeringMap::GetObjectsOfType(ObjectType type) const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (object->type() == type)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

std::vector<Object*> OrienteeringMap::GetObjectsOfType(uint8_t type) const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (object->type() & type)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

std::vector<Object*> OrienteeringMap::GetObstructingObjects() const
{
    std::vector<Object*> objects;

    for (auto& object: objects_)
    {
        if (object->symbol()->IsObstructing())
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
        if (!object->symbol()->IsObstructing() && !object->symbol()->IsIrrelevant() && object->type() == AreaO)
        {
            objects.emplace_back(object.get());
        }
    }
    return objects;
}

void OrienteeringMap::ClearObjectsOfType(ObjectType type)
{
    objects_.erase(std::ranges::remove_if(objects_,[type](const auto& object) {return object->type() == type;}).begin(), objects_.end());
}

void OrienteeringMap::ClearObjectsOfType(uint8_t type)
{
    objects_.erase(std::ranges::remove_if(objects_,[type](const auto& object) {return object->type() & type;}).begin(), objects_.end());
}

void OrienteeringMap::ClearObjectsOfFlag(SymbolFlag flag)
{
    objects_.erase(std::ranges::remove_if(objects_,[flag](const auto& object) {return object->symbol()->flags() & flag;}).begin(), objects_.end());
}

void OrienteeringMap::ClearObjectsOfFlag(uint8_t flag)
{
    objects_.erase(std::ranges::remove_if(objects_,[flag](const auto& object) {return object->symbol()->flags() & flag;}).begin(), objects_.end());
}

bool OrienteeringMap::ObjectsCanBeOrdered()
{
    return std::ranges::all_of(symbols_,[](auto symbol) {return symbol->HasColor(); });
}

OrienteeringMap::~OrienteeringMap()
{
    for (auto& color: colors_)
    {
        color.reset();
    }

    for (auto& symbol: symbols_)
    {
        symbol.reset();
    }

    for (auto& object: objects_)
    {
        object.reset();
    }

}
