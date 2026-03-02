//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_POINT_OBJECT_H
#define PROCEDURALFOLIAGEGENERATOR_POINT_OBJECT_H

#include "object.h"

namespace Orienteering
{
    class PointObject : public Object
    {

    public:
        PointObject() = delete;
        PointObject(Symbol*);
        void SetPoint(const Spatial2D& point);
    protected:
    private:


    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_POINT_OBJECT_H