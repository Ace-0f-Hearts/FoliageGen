//
// Created by ace on 2026-02-23.
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "ocad/ocad_importer.h"
#include "spatial/orienteering_path.h"
#include "spatial/poly_path.h"


BOOST_AUTO_TEST_SUITE(PolyPathTestSuite)

using namespace Spatial;

    BOOST_AUTO_TEST_CASE(Test_PolyPath_ConstructionFromBezier)
    {
        std::vector<ObjectCoordinate> bezier {{{{0,0},ObjectCoordinate::CurveStart},{{1,1}},{{0,2},},{{1,3},}}};
        PolyPath p_path;
        p_path.FromBezier(bezier,0);

        auto expected = 3;
        auto actual = p_path.size();
        BOOST_CHECK_EQUAL(expected,actual);

    };

BOOST_AUTO_TEST_SUITE_END()