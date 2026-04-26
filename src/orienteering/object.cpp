//
// Created by ace on 2026-01-29.
//

#include <iostream>
#include <list>
#include <orienteering/object.h>

#include <stdexcept>

Object::Object() : angle_(0), type_(UnknownO), symbol_(nullptr), coordinates_(false)
{
}

Object::Object(std::shared_ptr<Symbol> symbol) : angle_(0),
                                 type_(symbol->IsPath()
                                           ? PathO
                                           : symbol->IsArea()
                                           ? AreaO
                                           : symbol->IsPoint()
                                           ? PointO
                                           : UnknownO), symbol_(symbol), coordinates_(symbol->IsPath())
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


void Object::SetSymbol(std::shared_ptr<Symbol> symbol)
{
    symbol_ = symbol;

}

void Object::SetType(ObjectType type)
{
    if (type_ == PathO)
        coordinates().subdividing(true);
    type_ = type;
}

void Object::AppendCoordinate(Spatial::Spatial2D coordinate)
{
    UpdateBoundingBox();
}

bool Object::HasSymbolOf(const Symbol* symbol) const
{
    return *this->symbol_ == *symbol;
}

void Object::UpdateBoundingBox()
{
    bounding_box_ = coordinates_.ComputeBoundingBox();
}

ObjectType Object::type() const
{
    return type_;
}

const std::shared_ptr<Symbol> Object::symbol() const
{
    return symbol_;
}

Spatial::SegmentedPath Object::coordinates() const
{
    return coordinates_;
}

BoundingBox2D Object::bounding_box() const
{
    return bounding_box_;
}

std::list<Spatial::Spatial2D> Object::GetPoints() const
{
    return coordinates_.GetPoints();
}

Object::~Object()
{
    symbol_.reset();
}
