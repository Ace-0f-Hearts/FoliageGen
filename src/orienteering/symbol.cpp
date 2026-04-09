//
// Created by ace on 2026-01-29.
//
#include "../../include/orienteering/symbol.h"

#include <iostream>


void Orienteering::Symbol::name(const string& name)
{
    name_ = name;
}

void Orienteering::Symbol::number(uint32_t value)
{
    id_ = value;
}

Orienteering::string& Orienteering::Symbol::name()
{
    return name_;
}

Orienteering::string Orienteering::Symbol::name() const
{
    return name_;
}

uint32_t& Orienteering::Symbol::number()
{
    return id_;
}

uint32_t Orienteering::Symbol::number() const
{
    return id_;
}

bool Orienteering::Symbol::operator==(const Symbol& other) const
{
    return id_ == other.id_;
}

uint8_t Orienteering::Symbol::flags() const
{
    return flags_;
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

uint32_t Orienteering::Symbol::id() const
{
    return id_;
}

void Orienteering::Symbol::id(uint32_t value)
{
    id_ = value;
}

void Orienteering::Symbol::type(SymbolType type)
{
    type_ = type;
}

Orienteering::SymbolType Orienteering::Symbol::type() const
{
    return type_;
}

bool Orienteering::Symbol::IsIrrelevant() const
{
    return flags_ & Irrelevant;
}

void Orienteering::Symbol::SetMapSymbol(bool mapsymbol)
{
    if (mapsymbol)
        flags_ |= Irrelevant;
    else
        flags_ &= ~Irrelevant;
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

const MapColor* Orienteering::Symbol::color() const
{
    return color_;
}

void Orienteering::Symbol::SetColor(const MapColor* color)
{
    color_ = color;
}

Orienteering::Symbol::Symbol() : id_(0), flags_(0), type_()
{
}
