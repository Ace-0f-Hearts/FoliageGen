//
// Created by ace on 2026-01-29.
//

#include "../../include/orienteering/object.h"

#include <stdexcept>

#include "../../include/utility/not_implemented_error.h"

Orienteering::Object::Object(Symbol* symbol) : type_(), symbol_(symbol)
{
}

Orienteering::ObjectType Orienteering::Object::type() const
{
    return this->type_;
}

const std::vector<Spatial2D>& Object::coordinates() const
{
    throw NotImplementedError();
}

std::vector<Spatial2D>& Object::coordinates()
{
    throw NotImplementedError();
}

const Symbol* Object::symbol() const
{
    return symbol_;
}

