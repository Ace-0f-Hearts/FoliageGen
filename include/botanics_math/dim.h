//
// Created by ace on 2026-02-26.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_DIM_H
#define PROCEDURALFOLIAGEGENERATOR_DIM_H

struct Dim2
{
    int x;
    int y;
    Dim2 operator*(float scalar) const
    {
        return Dim2(x * scalar,y * scalar);
    };
    void operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
    }
};

#endif //PROCEDURALFOLIAGEGENERATOR_DIM_H