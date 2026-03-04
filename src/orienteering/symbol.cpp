//
// Created by ace on 2026-01-29.
//
#include "../../include/orienteering/symbol.h"

#include <assert.h>
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
    return flags_ & AreaS;
}

bool Orienteering::Symbol::IsPath() const
{
    return flags_ & PathS;
}

bool Orienteering::Symbol::IsPoint() const
{
    return flags_ & PointS;
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

uint32_t Orienteering::Symbol::id()
{
    return id_;
}

void Orienteering::Symbol::id(uint32_t value)
{
    id_ = value;
}
