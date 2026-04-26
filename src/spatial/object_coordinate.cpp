//
// Created by ace on 2026-02-23.
//

#include <spatial/object_coordinate.h>

ObjectCoordinate::ObjectCoordinate(Spatial::Spatial2D coordinate, uint8_t flags) : coordinate_(coordinate), flags_(flags)
{
}

ObjectCoordinate::ObjectCoordinate(Spatial::Spatial2D& coordinate, uint8_t flags) : coordinate_(coordinate), flags_(flags)
{
}

ObjectCoordinate::ObjectCoordinate(const ObjectCoordinate& coordinate) : coordinate_(coordinate.coordinate_), flags_(coordinate.flags_)
{
}


bool ObjectCoordinate::IsHolePoint() const
{
    return flags_ & HolePoint;
}

void ObjectCoordinate::SetClosePoint(const bool set)
{
    if (set)
        flags_ |= ClosePoint;
    else
        flags_ &= ~ClosePoint;
}

void ObjectCoordinate::SetHolePoint(const bool set)
{
    if (set)
        flags_ |= HolePoint;
    else
        flags_ &= ~HolePoint;
}

void ObjectCoordinate::SetCurveStart(const bool set)
{
    if (set)
        flags_ |= CurveStart;
    else
        flags_ &= ~CurveStart;
}

bool ObjectCoordinate::PositionEqualTo(const Spatial::Spatial2D& coordinate) const
{
    return coordinate_ == coordinate;
}

bool ObjectCoordinate::PositionEqualTo(const ObjectCoordinate& rhs) const
{
    return coordinate_ == rhs.coordinate_;
}

void ObjectCoordinate::SetFlags(const uint8_t flags)
{
    flags_ = flags;
}

bool ObjectCoordinate::IsCurveStart() const
{
    return flags_ & CurveStart;
}

uint8_t& ObjectCoordinate::flags()
{
    return flags_;
}

Spatial::Spatial2D& ObjectCoordinate::coordinate()
{
    return coordinate_;
}

uint8_t ObjectCoordinate::flags() const
{
    return flags_;
}

Spatial::Spatial2D ObjectCoordinate::coordinate() const
{
    return coordinate_;
}

void ObjectCoordinate::SetGapPoint(bool set)
{
    if (set)
        flags_ |= GapPoint;
    else
        flags_ &= ~GapPoint;

}

void ObjectCoordinate::SetDashPoint(bool set)
{
    if (set)
        flags_ |= DashPoint;
    else
        flags_ &= ~DashPoint;
}

bool ObjectCoordinate::IsClosePoint() const
{
    return flags_ & ClosePoint;
}

bool ObjectCoordinate::IsGapPoint() const
{
    return flags_ & GapPoint;
}

bool ObjectCoordinate::IsDashPoint() const
{
    return flags_ & DashPoint;
}
