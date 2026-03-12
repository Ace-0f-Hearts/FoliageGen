//
// Created by ace on 2026-01-29.
//

#ifndef GENERATOR_OBJECT_H
#define GENERATOR_OBJECT_H

#include "symbol.h"
#include <spatial/poly_path.h>

#include "spatial/bounding_box.h"
#include "spatial/segmented_path.h"


using namespace Orienteering;


namespace Orienteering
{

    enum ObjectType
    {
        PointO = 0x1,
        PathO = 0x2,
        AreaO = 0x4,
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
        void AppendCoordinate(Spatial::Spatial2D coordinate);

        bool HasSymbolOf(const Symbol* symbol) const;

        virtual bool IsIntersecting(const Spatial::Spatial2D& point) = 0;

        [[nodiscard]] ObjectType type() const;

        [[nodiscard]] const Symbol* symbol() const;

    protected:
        void UpdateBoundingBox();

        float angle_;
        ObjectType type_;
        Symbol* symbol_;

        float distance_threshold_ = 0.f;
        Spatial::SegmentedPath coordinates_;
        BoundingBox2D bounding_box_;

    };

}


#endif //GENERATOR_OBJECT_H