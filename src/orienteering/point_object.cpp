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
    coordinates_.Clear();
    coordinates_.SetPoint(point);
}



PointObject::~PointObject()
{
}

bool PointObject::IsIntersecting(const Spatial2D& point)
{
    return false;
}
