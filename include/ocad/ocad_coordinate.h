//
// Created by ace on 2026-02-16.

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
#include <cstdint>

#include <spatial/spatial_coordinates.h>



/**
 * Used to describe vertices of OCAD objects, like areas, paths, single points, etc.
 * Has a number of flags which can be set to indicate the type of point.
 */

class OcadCoordinate
{
public:

    enum Flag
    {
        CurveStart = 1 << 0,
        ClosePoint = 1 << 1,
        GapPoint = 1 << 2,
        HolePoint = 1 << 4,
        DashPoint = 1 << 5,
    };


    OcadCoordinate() = default;
    OcadCoordinate(Spatial2D& coordinate, uint8_t flags = 0);
    OcadCoordinate(Spatial2D coordinate, uint8_t flags = 0);
    OcadCoordinate(const OcadCoordinate& coordinate);

    void SetClosePoint(bool set);
    void SetHolePoint(bool set);
    void SetCurveStart(bool set);
    void SetGapPoint(bool set);
    void SetDashPoint(bool set);

    [[nodiscard]] bool PositionEqualTo(const Spatial2D& coordinate ) const;
    [[nodiscard]] bool PositionEqualTo(const OcadCoordinate& rhs) const;

    void SetFlags(uint8_t flags);
    [[nodiscard]] bool IsHolePoint() const;
    [[nodiscard]] bool IsClosePoint() const;
    [[nodiscard]] bool IsGapPoint() const;
    [[nodiscard]] bool IsDashPoint() const;
    [[nodiscard]] bool IsCurveStart() const;

    uint8_t& flags();
    Spatial2D& coordinate();
private:
    Spatial2D coordinate_;
    uint8_t flags_ = 0;
};

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
