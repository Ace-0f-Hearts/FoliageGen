//
// Created by ace on 2026-02-23.
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <ocad/ocad_coordinate.h>


BOOST_AUTO_TEST_SUITE(OcadCoordinateTestSuite)

BOOST_AUTO_TEST_CASE(Test_OcadCoordiante_FlagsSetCorrectly)
{
    OcadCoordinate close_point, curve_start, hole_point, dash_point, gap_point;
    close_point.SetClosePoint(true);
    hole_point.SetHolePoint(true);
    dash_point.SetDashPoint(true);
    gap_point.SetGapPoint(true);
    curve_start.SetCurveStart(true);

    BOOST_CHECK(close_point.IsClosePoint());
    BOOST_CHECK(gap_point.IsGapPoint());
    BOOST_CHECK(dash_point.IsDashPoint());
    BOOST_CHECK(hole_point.IsHolePoint());
    BOOST_CHECK(curve_start.IsCurveStart());
};

BOOST_AUTO_TEST_CASE(Test_OcadCoordinate_AllFlagsSet)
{
    OcadCoordinate point;
    point.SetClosePoint(true);
    point.SetCurveStart(true);
    point.SetDashPoint(true);
    point.SetGapPoint(true);
    point.SetHolePoint(true);

    BOOST_CHECK(point.IsClosePoint());
    BOOST_CHECK(point.IsCurveStart());
    BOOST_CHECK(point.IsDashPoint());
    BOOST_CHECK(point.IsGapPoint());
    BOOST_CHECK(point.IsHolePoint());
}

BOOST_AUTO_TEST_CASE(Test_OcadCoordinate_FlagUnsetsCorrectly)
{
    OcadCoordinate close_point, curve_start, hole_point, dash_point, gap_point;
    close_point.SetClosePoint(true);
    hole_point.SetHolePoint(true);
    dash_point.SetDashPoint(true);
    gap_point.SetGapPoint(true);
    curve_start.SetCurveStart(true);

    close_point.SetClosePoint(false);
    hole_point.SetHolePoint(false);
    dash_point.SetDashPoint(false);
    gap_point.SetGapPoint(false);
    curve_start.SetCurveStart(false);

    BOOST_CHECK(!close_point.IsClosePoint());
    BOOST_CHECK(!gap_point.IsGapPoint());
    BOOST_CHECK(!dash_point.IsDashPoint());
    BOOST_CHECK(!hole_point.IsHolePoint());
    BOOST_CHECK(!curve_start.IsCurveStart());
}

BOOST_AUTO_TEST_CASE(Test_OcadCoordinate_FlagsSetsAtConstruction)
{
    OcadCoordinate point {Spatial::Spatial2D({0,0}),OcadCoordinate::GapPoint | OcadCoordinate::ClosePoint | OcadCoordinate::HolePoint | OcadCoordinate::DashPoint | OcadCoordinate::CurveStart};

    BOOST_CHECK(point.IsClosePoint());
    BOOST_CHECK(point.IsCurveStart());
    BOOST_CHECK(point.IsDashPoint());
    BOOST_CHECK(point.IsGapPoint());
    BOOST_CHECK(point.IsHolePoint());
    point.SetClosePoint(false);
    point.SetHolePoint(false);
    point.SetDashPoint(false);
    point.SetGapPoint(false);
    point.SetCurveStart(false);

    BOOST_CHECK(!point.IsClosePoint());
    BOOST_CHECK(!point.IsGapPoint());
    BOOST_CHECK(!point.IsDashPoint());
    BOOST_CHECK(!point.IsHolePoint());
    BOOST_CHECK(!point.IsCurveStart());
}

BOOST_AUTO_TEST_SUITE_END()