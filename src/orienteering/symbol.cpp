//
// Created by ace on 2026-01-29.
//
#include "../../include/orienteering/symbol.h"

#include <cassert>
#include <iostream>

#include "loguru.hpp"


Orienteering::Symbol::Symbol() : id_(0), flags_(0), radius_of_influence_(0.f), type_()
{
}

void Orienteering::Symbol::SetName(const string& name)
{
    name_ = name;
}

void Orienteering::Symbol::SetNumber(uint32_t value)
{
    id_ = value;
}

Orienteering::string& Orienteering::Symbol::GetName()
{
    return name_;
}

Orienteering::string Orienteering::Symbol::GetName() const
{
    return name_;
}

uint32_t& Orienteering::Symbol::GetNumber()
{
    return id_;
}

uint32_t Orienteering::Symbol::GetNumber() const
{
    return id_;
}

bool Orienteering::Symbol::operator==(const Symbol& other) const
{
    return id_ == other.id_;
}

uint32_t Orienteering::Symbol::GetId() const
{
    return id_;
}

void Orienteering::Symbol::SetId(uint32_t value)
{
    id_ = value;
}

void Orienteering::Symbol::SetRadiusOfInfluence(float radius)
{
    radius_of_influence_ = radius;
}

float Orienteering::Symbol::GetRadiusOfInfluence() const
{
    return radius_of_influence_;
}

const MapColor* Orienteering::Symbol::GetColor() const
{
    return color_.get();
}

uint8_t Orienteering::Symbol::flags() const
{
    return flags_;
}

bool Orienteering::Symbol::HasColor() const
{
    return color_.get();
}

bool Orienteering::Symbol::IsArea() const
{
    return type_ == AreaS;
}

bool Orienteering::Symbol::IsPath() const
{
    return type_ == PathS;
}

bool Orienteering::Symbol::IsPoint() const
{
    return type_ == PointS;
}

bool Orienteering::Symbol::IsIrrelevant() const
{
    return flags_ & Irrelevant;
}

bool Orienteering::Symbol::IsCultivated() const
{
    return flags_ & Cultivated;
}

bool Orienteering::Symbol::IsObstructing() const
{
    return flags_ & Obstructing;
}

bool Orienteering::Symbol::IsDirectional() const
{
    return flags_ & Directional;
}

void Orienteering::Symbol::type(SymbolType type)
{
    type_ = type;
}

Orienteering::SymbolType Orienteering::Symbol::type() const
{
    return type_;
}

void Orienteering::Symbol::SetCultivated(bool cultivated)
{
    if (cultivated)
        flags_ |= Cultivated;
    else
        flags_ &= ~Cultivated;
}

void Orienteering::Symbol::SetObstructing(bool obstructing)
{
    if (obstructing)
        flags_ |= Obstructing;
    else
        flags_ &= ~Obstructing;
}

void Orienteering::Symbol::SetDirectional(bool directional)
{
    if (directional)
        flags_ |= Directional;
    else
        flags_ &= ~Directional;
}

void Orienteering::Symbol::SetMapSymbol(bool mapsymbol)
{
    if (mapsymbol)
        flags_ |= Irrelevant;
    else
        flags_ &= ~Irrelevant;
}

void Orienteering::Symbol::SetColor(const MapColor* color)
{
    assert(color);
    color_ = std::make_shared<MapColor>(*color);
}

void Orienteering::Symbol::SetColor(MapColor* color)
{
    assert(color);
    color_ = std::make_shared<MapColor>(*color);
}

void Orienteering::Symbol::SetColor(std::shared_ptr<MapColor>& color)
{
    color_ = color;
}
