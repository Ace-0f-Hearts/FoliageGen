//
// Created by ace on 2026-01-29.
//

#include <orienteering/object.h>

#include <stdexcept>

Object::Object(const Object& other) : type_(other.type_), symbol_(other.symbol_)
{
}

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

void Object::SetSymbol(Symbol* symbol)
{
}

void Object::SetType(ObjectType type)
{
}

void Object::AppendCoordinate(Spatial2D coordinate)
{
}

Object::Object() : type_(), symbol_(nullptr)
{
}

bool Object::HasSymbolOf(const Symbol* symbol) const
{
    return this->symbol_ == symbol;
}

Object::Object(const Object&& other)  noexcept : type_(other.type_), symbol_(other.symbol_)
{
}

Object& Object::operator=(const Object& other)
{
    if (this != &other)
    {
        this->type_ = other.type_;
        this->symbol_ = other.symbol_;
    }
    return *this;
}

Object& Object::operator=(Object&& other) noexcept
{
    if (this != &other)
    {
        this->type_ = other.type_;
        this->symbol_ = other.symbol_;
    }
    return *this;
}
