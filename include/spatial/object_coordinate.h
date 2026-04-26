//
// Created by ace on 2026-02-16.

#ifndef PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
#define PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
#include <cstdint>

#include <spatial/spatial_coordinate.h>



/**
 * Used to describe vertices of OCAD objects, like areas, paths, single points, etc.
 * Has a number of flags which can be set to indicate the type of point.
 */

class ObjectCoordinate
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


    ObjectCoordinate() = default;
    ObjectCoordinate(Spatial::Spatial2D& coordinate, uint8_t flags = 0);
    ObjectCoordinate(Spatial::Spatial2D coordinate, uint8_t flags = 0);
    ObjectCoordinate(const ObjectCoordinate& coordinate);

    void SetClosePoint(bool set);
    void SetHolePoint(bool set);
    void SetCurveStart(bool set);
    void SetGapPoint(bool set);
    void SetDashPoint(bool set);

    [[nodiscard]] bool PositionEqualTo(const Spatial::Spatial2D& coordinate ) const;
    [[nodiscard]] bool PositionEqualTo(const ObjectCoordinate& rhs) const;

    void SetFlags(uint8_t flags);
    [[nodiscard]] bool IsHolePoint() const;
    [[nodiscard]] bool IsClosePoint() const;
    [[nodiscard]] bool IsGapPoint() const;
    [[nodiscard]] bool IsDashPoint() const;
    [[nodiscard]] bool IsCurveStart() const;

    uint8_t& flags();
    uint8_t flags() const;
    Spatial::Spatial2D& coordinate();
    Spatial::Spatial2D coordinate() const;

private:
    Spatial::Spatial2D coordinate_;
    uint8_t flags_ = 0;
};

inline std::ostream& operator<<(std::ostream& os, const ObjectCoordinate& coordinate)
{
    os << coordinate.coordinate();
    return os;
}

#endif //PROCEDURALFOLIAGEGENERATOR_OCAD_COORDINATE_H
