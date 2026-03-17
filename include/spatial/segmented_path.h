//
// Created by ace on 2026-02-23.

#ifndef PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#define PROCEDURALFOLIAGEGENERATOR_PATH_PART_H
#include <vector>

#include "bounding_box.h"
#include "poly_path.h"


namespace Spatial
{
    /**
    * Holds a collection of paths that are used to handle areas which might have holes in them
    */
    class SegmentedPath
    {
    public:
        SegmentedPath(bool subdividing);
        [[nodiscard]] bool IsPointInsideArea(const Spatial2D& point) const;
        [[nodiscard]] bool IsPointOnPath(const Spatial2D& point, float distance_threshold = 0.f) const;

        [[nodiscard]] bool Contains(const Spatial2D& point) const;
        [[nodiscard]] bool IsClosed() const;


        BoundingBox<2> ComputeBoundingBox() const;
        void SetPoints(const std::vector<Spatial2D>& points);
        void SetPoint(const Spatial2D& points);
        void AppendPoint(const Spatial2D& point);
        void RemovePoint(const Spatial2D& point);
        void Clear();

        void FromBezier(std::vector<OcadCoordinate>& curve);

        [[nodiscard]] const std::vector<PolyPath>& paths() const;
        std::vector<PolyPath>& paths();

        void subdividing(bool enable);
    private:
        constexpr static float kMax_segment_length = 10000000.0f;

        void Subdivide(float max_segment_length = kMax_segment_length);
        Spatial::PolyPath SubdividePart(PolyPath& path,float max_segment_length = kMax_segment_length);

        bool subdividing_ = false;
        std::vector<PolyPath> paths_;
    };
}



#endif //PROCEDURALFOLIAGEGENERATOR_PATH_PART_H