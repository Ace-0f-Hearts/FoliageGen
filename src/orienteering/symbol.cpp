//
// Created by ace on 2026-01-29.
//
#include "../../include/orienteering/symbol.h"

#include <assert.h>




void Orienteering::Symbol::name(string& name)
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
