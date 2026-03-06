//
// Created by ace on 2026-03-01.
//
#include <orienteering/point_object.h>

PointObject::PointObject(Symbol* symbol) : Object(symbol)
{
    type_ = PointO;
}

void PointObject::SetPoint(const Spatial2D& point)
{
    this->coordinates().clear();
    this->coordinates().push_back(point);
}

PointObject::~PointObject()
{
}

bool PointObject::IsIntersecting(const Spatial2D& point)
{
    return false;
}
