//
// Created by ace on 2026-02-16.
//

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H
#include "object.h"

namespace Orienteering
{

    class PathObject : public Object
    {
    public:
        PathObject() = delete;
        PathObject(Symbol* symbol);

        ~PathObject() override = default;

        [[nodiscard]] bool IsClosed() const;
        [[nodiscard]] bool Contains(const Spatial::Spatial2D& point) const;
        bool IsIntersecting(const Spatial::Spatial2D& point) override;

        void BuildCurve(std::vector<OcadCoordinate>& curve);
        void SetCurve(const std::vector<Spatial::Spatial2D>& points);
        void AppendPoint(const Spatial::Spatial2D& point);
        void RemovePoint(const Spatial::Spatial2D& point);
        void Clear();
    };
}

#endif //PROCEDURALFOLIAGEGENERATOR_PATH_OBJECT_H