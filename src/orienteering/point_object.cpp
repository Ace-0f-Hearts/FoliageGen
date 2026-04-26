//
// Created by ace on 2026-03-01.
//
#include <orienteering/point_object.h>

#include "loguru.hpp"

PointObject::PointObject(std::shared_ptr<Symbol> symbol) : Object(symbol)
{
}

void PointObject::SetPoint(const Spatial::Spatial2D& point)
{
    coordinates_.Clear();
    coordinates_.SetPoint(point);
    auto min =  coordinates().paths()[0].points()[0];
    auto max = coordinates().paths()[0].points()[0];

    float radius = symbol_->GetRadiusOfInfluence();

    min -= Spatial::Spatial2D({radius, radius});
    max += Spatial::Spatial2D({radius, radius});
    bounding_box_ = BoundingBox2D{min,max};
}



PointObject::~PointObject() = default;

bool PointObject::IsIntersecting(const Spatial::Spatial2D& point) const
{
    auto object_point = coordinates().paths()[0].points()[0];

    auto distance = object_point.DistanceTo(point);
    return distance < symbol()->GetRadiusOfInfluence();
}



