//
// Created by ace on 2026-03-23.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H
#define PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H

#include <fstream>

struct Cmyk
{
    Cmyk() : c(0), m(0), y(0), k(0) {};
    Cmyk(float c = 0.f, float m = 0.f, float y = 0.f, float k = 0.f) : c(c),m(m),y(y),k(k) {};

    ~Cmyk()  = default;

    [[nodiscard]] bool IsWhite() const;

    float c;
    float m;
    float y;
    float k;
};

class MapColor
{
public:
    MapColor() : cmyk_(0,0,0,0),priority_(-1), opacity_(1.f) {};
    MapColor(const Cmyk cmyk, const int priority = -1.f, const float opacity = 1.f) : cmyk_(cmyk), priority_(priority), opacity_(opacity) {};

    ~MapColor() = default;

    [[nodiscard]] bool IsWhite() const;

    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] Cmyk GetCmyk() const;
    [[nodiscard]] int GetPriority() const;
    [[nodiscard]] float GetOpacity() const;

    void SetName(const std::string& name);
    void SetCmyk(const Cmyk& cmyk);
    void SetPriority(const int priority);
    void SetOpacity(const float opacity);
private:

    std::string name_;
    Cmyk cmyk_;
    int priority_;
    float opacity_;
};

inline bool Cmyk::IsWhite() const
{
    return c == 0.f && m == 0.f && y == 0.f && k == 0.f;
}

inline bool MapColor::IsWhite() const
{
    return cmyk_.IsWhite();
}

inline std::ostream& operator<<(std::ostream& os, const Cmyk& cmyk)
{
    os << "<" << cmyk.c << "," << cmyk.m << "," << cmyk.y << ">";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const MapColor& map_color)
{
    os << "(" << map_color.GetName() << ";" << map_color.GetCmyk() << ";" << map_color.GetPriority() << ";" << map_color.GetOpacity() << ")";
    return os;
}


#endif //PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H
