//
// Created by ace on 2026-02-23.
//

#include <ocad/ocad_coordinate.h>

OcadCoordinate::OcadCoordinate(Spatial2D coordinate, uint8_t flags) : coordinate_(coordinate), flags_(flags)
{
}

OcadCoordinate::OcadCoordinate(Spatial2D& coordinate, uint8_t flags) : coordinate_(coordinate), flags_(flags)
{
}

OcadCoordinate::OcadCoordinate(const OcadCoordinate& coordinate) : coordinate_(coordinate.coordinate_), flags_(coordinate.flags_)
{
}


bool OcadCoordinate::IsHolePoint() const
{
    return flags_ & HolePoint;
}

void OcadCoordinate::SetClosePoint(const bool set)
{
    if (set)
        flags_ |= ClosePoint;
    else
        flags_ &= ~ClosePoint;
}

void OcadCoordinate::SetHolePoint(const bool set)
{
    if (set)
        flags_ |= HolePoint;
    else
        flags_ &= ~HolePoint;
}

void OcadCoordinate::SetCurveStart(const bool set)
{
    if (set)
        flags_ |= CurveStart;
    else
        flags_ &= ~CurveStart;
}

bool OcadCoordinate::PositionEqualTo(const Spatial2D& coordinate) const
{
    return coordinate_ == coordinate;
}

bool OcadCoordinate::PositionEqualTo(const OcadCoordinate& rhs) const
{
    return coordinate_ == rhs.coordinate_;
}

void OcadCoordinate::SetFlags(const uint8_t flags)
{
    flags_ = flags;
}

bool OcadCoordinate::IsCurveStart() const
{
    return flags_ & CurveStart;
}

uint8_t& OcadCoordinate::flags()
{
    return flags_;
}

Spatial2D& OcadCoordinate::coordinate()
{
    return coordinate_;
}

void OcadCoordinate::SetGapPoint(bool set)
{
    if (set)
        flags_ |= GapPoint;
    else
        flags_ &= ~GapPoint;

}

void OcadCoordinate::SetDashPoint(bool set)
{
    if (set)
        flags_ |= DashPoint;
    else
        flags_ &= ~DashPoint;
}

bool OcadCoordinate::IsClosePoint() const
{
    return flags_ & ClosePoint;
}

bool OcadCoordinate::IsGapPoint() const
{
    return flags_ & GapPoint;
}

bool OcadCoordinate::IsDashPoint() const
{
    return flags_ & DashPoint;
}
