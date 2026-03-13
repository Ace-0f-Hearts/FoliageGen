//
// Created by ace on 2026-01-29.
//

#include <iostream>
#include <orienteering/object.h>

#include <stdexcept>

Object::Object() : angle_(0), type_(UnknownO), symbol_(nullptr)
{
}

Object::Object(Symbol* symbol) : angle_(0), type_(symbol->IsPath() ? PathO : symbol->IsArea() ? AreaO : symbol->IsPoint() ? PointO : UnknownO), symbol_(symbol)
{
}

Object::Object(const Object& other) : angle_(other.angle_), type_(other.type_), symbol_(other.symbol_), coordinates_(other.coordinates_)
{
    UpdateBoundingBox();
}

Object::Object(const Object&& other)  noexcept : angle_(0), type_(other.type_), symbol_(other.symbol_), coordinates_(other.coordinates_)
{
    UpdateBoundingBox();
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


void Object::SetSymbol(Symbol* symbol)
{
    symbol_ = symbol;
}

void Object::SetType(ObjectType type)
{
    type_ = type;
}

void Object::AppendCoordinate(Spatial::Spatial2D coordinate)
{
    UpdateBoundingBox();
}

bool Object::HasSymbolOf(const Symbol* symbol) const
{
    return this->symbol_ == symbol;
}

ObjectType Object::type() const
{
    return type_;
}

const Symbol* Object::symbol() const
{
    return symbol_;
}

void Object::UpdateBoundingBox()
{
    bounding_box_ = coordinates_.ComputeBoundingBox();
}
