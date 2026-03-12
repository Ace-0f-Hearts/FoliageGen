//
// Created by ace on 2026-02-23.

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#include <vector>

#include "poly_path.h"


namespace Spatial
{
    /**
    * Holds a collection of paths that are used to handle areas which might have holes in them
    */
    class PathCollection
    {
    public:
        [[nodiscard]] bool IsPointInsideArea(const Spatial2D& point) const;
        [[nodiscard]] bool IsPointOnPath(const Spatial2D& point) const;
        [[nodiscard]] bool Contains(const Spatial2D& point) const;

        void SetPoints(const std::vector<Spatial2D>& points);
        void SetPoint(const Spatial2D& points);
        void AppendPoint(const Spatial2D& point);
        void RemovePoint(const Spatial2D& point);
        void Clear();

        void FromBezier(std::vector<OcadCoordinate>& curve);

        [[nodiscard]] const std::vector<PolyPath>& paths() const;
        std::vector<PolyPath>& paths();
    private:
        constexpr static float kMax_segment_length = 10000000.0f;

        void Subdivide(float max_segment_length = kMax_segment_length);
        Spatial::PolyPath SubdividePart(PolyPath& path,float max_segment_length = kMax_segment_length);


        std::vector<PolyPath> paths_;
    };
}



#endif //PROCEDURALFOLIAGEGENERATOR_PATH_PART_H