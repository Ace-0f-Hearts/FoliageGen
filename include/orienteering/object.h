//
// Created by ace on 2026-01-29.
//

#ifndef GENERATOR_OBJECT_H
#define GENERATOR_OBJECT_H

#include "symbol.h"
#include <spatial/poly_path.h>

using namespace Orienteering;

class PolyPath;

namespace Orienteering
{

    enum ObjectType
    {
        Point = 0,
        Path = 1
    };

    class Object
    {
    public:
        Object(Symbol* symbol);
        virtual ~Object() = default;

        virtual bool IsIntersecting(const Spatial2D& point) = 0;

        [[nodiscard]] ObjectType type() const;
        [[nodiscard]] const std::vector<Spatial2D>& coordinates() const;
        [[nodiscard]] std::vector<Spatial2D>& coordinates();
        [[nodiscard]] const Symbol* symbol() const;

    protected:
        ObjectType type_;
        Symbol* symbol_;
        Spatial::PolyPath coordinates_;
    private:
    };

}


#endif //GENERATOR_OBJECT_H