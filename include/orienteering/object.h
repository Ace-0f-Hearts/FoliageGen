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
        UnknownO = 0x8,
    };

    class Object
    {
    public:
        Object();
        Object(std::shared_ptr<Symbol> symbol);
        Object(const Object& other);
        Object(const Object&& other) noexcept;

        Object& operator=(const Object& other);
        Object& operator=(Object&& other) noexcept;
        virtual ~Object();

        [[nodiscard]] std::list<Spatial::Spatial2D> GetPoints() const;

        void SetSymbol(std::shared_ptr<Symbol> symbol);
        void SetType(ObjectType type);
        void AppendCoordinate(Spatial::Spatial2D coordinate);

        bool HasSymbolOf(const Symbol* symbol) const;

        virtual bool IsIntersecting(const Spatial::Spatial2D& point) const = 0;

        [[nodiscard]] ObjectType type() const;
        [[nodiscard]] const std::shared_ptr<Symbol> symbol() const;
        [[nodiscard]] Spatial::SegmentedPath coordinates() const;
        [[nodiscard]] BoundingBox2D bounding_box() const;

    protected:
        void UpdateBoundingBox();

        float angle_;
        ObjectType type_;
        std::shared_ptr<Symbol> symbol_;

        Spatial::SegmentedPath coordinates_;
        BoundingBox2D bounding_box_;

    };

}


#endif //GENERATOR_OBJECT_H