//
// Created by ace on 2026-01-29.
//

#ifndef GENERATOR_OBJECT_H
#define GENERATOR_OBJECT_H

#include "symbol.h"
#include <spatial/poly_path.h>
#include "spatial/path_collection.h"


using namespace Orienteering;


namespace Orienteering
{

    enum ObjectType
    {
        PointO = 0,
        PathO = 1
    };

    class Object
    {
    public:
        Object();
        Object(Symbol* symbol);
        Object(const Object& other);
        Object(const Object&& other) noexcept;

        Object& operator=(const Object& other);
        Object& operator=(Object&& other) noexcept;
        virtual ~Object() = default;

        void SetSymbol(Symbol* symbol);
        void SetType(ObjectType type);
        void AppendCoordinate(Spatial2D coordinate);

        bool HasSymbolOf(const Symbol* symbol) const;

        virtual bool IsIntersecting(const Spatial2D& point) = 0;

        [[nodiscard]] ObjectType type() const;

        [[nodiscard]] const Symbol* symbol() const;

    protected:
        float angle_;
        ObjectType type_;
        Symbol* symbol_;
        Spatial::PathCollection coordinates_;
    };

}


#endif //GENERATOR_OBJECT_H