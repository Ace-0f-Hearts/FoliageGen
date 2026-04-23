//
// Created by ace on 2026-04-12.
//
#include <orienteering//map_color.h>

std::string MapColor::GetName() const
{
    return name_;
}

Cmyk MapColor::GetCmyk() const
{
    return cmyk_;
}

int MapColor::GetPriority() const
{
    return priority_;
}

float MapColor::GetOpacity() const
{
    return opacity_;
}

void MapColor::SetName(const std::string& name)
{
    name_ = name;
}

void MapColor::SetCmyk(const Cmyk& cmyk)
{
    cmyk_ = cmyk;
}

void MapColor::SetPriority(const int priority)
{
    priority_ = priority;
}

void MapColor::SetOpacity(const float opacity)
{
    opacity_ = opacity;
}
