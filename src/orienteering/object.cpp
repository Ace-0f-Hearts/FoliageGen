//
// Created by ace on 2026-01-29.
//

#include <orienteering/object.h>

#include <stdexcept>


Object::Object(Symbol* symbol) : type_(), symbol_(symbol)
{
}

ObjectType Object::type() const
{
    return this->type_;
}

const std::vector<Spatial2D>& Object::coordinates() const
{
    return this->coordinates_.points();
}

std::vector<Spatial2D>& Object::coordinates()
{
    return this->coordinates_.points();
}

const Symbol* Object::symbol() const
{
    return symbol_;
}

