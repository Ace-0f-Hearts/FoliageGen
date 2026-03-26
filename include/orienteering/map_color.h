//
// Created by ace on 2026-03-23.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H
#define PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H

struct Cmyk
{
    Cmyk() : c(0), m(0), y(0), k(0) {};
    Cmyk(float c = 0.f, float m = 0.f, float y = 0.f, float k = 0.f) : c(c),m(m),y(y),k(k) {};

    ~Cmyk()  = default;

    float c;
    float m;
    float y;
    float k;
};

struct MapColor
{
    MapColor() : cmyk(0,0,0,0),priority(-1), opacity(1.f) {};
    MapColor(Cmyk cmyk, int priority = -1.f, float opacity = 1.f) : cmyk(cmyk), priority(priority), opacity(opacity) {};

    ~MapColor() = default;

    Cmyk cmyk;
    int priority;
    float opacity;
};



#endif //PROCEDURALFOLIAGEGENERATOR_MAP_COLOR_H