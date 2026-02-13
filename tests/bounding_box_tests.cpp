//
// Created by ace on 2026-02-13.
//


#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <spatial/bounding_box.h>
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(BoundingBoxTestingSuite)

BOOST_AUTO_TEST_CASE(Test_BoundingBox_InvalidConstruction)
{
    BOOST_CHECK_THROW(BoundingBox(Spatial2D({2,2}),Spatial2D({1,2})), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(Test_BoundingBox2D_DistanceFromPointProducesExpectedDistance,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    const Spatial2D p1 = {1,-5};
    const Spatial2D p2 = {-3,-5};
    const Spatial2D p3 = {-1,-2.4};
    const Spatial2D p4 = {0,0.1};
    BoundingBox2D bounding_box(Spatial2D{-1,-2.4},Spatial2D({2,2.9}));

    const float exp1 = std::sqrt(2.6f * 2.6f);
    const float exp2 = std::sqrt(2.f*2.f + 2.6f * 2.6f);
    const float exp3 = 0;
    const float exp4 = 0;

    const auto act1 = bounding_box.CalculateDistance(p1);
    const auto act2 = bounding_box.CalculateDistance(p2);
    const auto act3 = bounding_box.CalculateDistance(p3);
    const auto act4 = bounding_box.CalculateDistance(p4);

    BOOST_CHECK_EQUAL(exp1,act1);
    BOOST_CHECK_EQUAL(exp2,act2);
    BOOST_CHECK_EQUAL(exp3,act3);
    BOOST_CHECK_EQUAL(exp4,act4);
}

BOOST_AUTO_TEST_CASE(Test_BoundingBox2D_SmallestBoundingCircleRadiusEqualsExpectedValue,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    BoundingBox2D bounding_box = {{-1,-1},{2,3}};

    const float expected = std::sqrt(3.f*3.f + 4.f*4.f) / 2.f;

    const auto actual = bounding_box.GetSmallestBoundingCircleRadius();

    BOOST_CHECK_EQUAL(expected,actual);
}

BOOST_AUTO_TEST_CASE(Test_BoundingBox2D_SubspaceCalculation)
{
    BoundingBox2D bounding_box = {{-1,-1},{1,1}};

    auto zeroth_quadrant = bounding_box.CalculateSubspaceOf({0.5,0.5});
    auto first_quadrant = bounding_box.CalculateSubspaceOf({-0.5,0.5});
    auto second_quadrant = bounding_box.CalculateSubspaceOf({0.5,-0.5});
    auto third_quadrant = bounding_box.CalculateSubspaceOf({-0.5,-0.5});

    BOOST_CHECK_EQUAL(zeroth_quadrant,BoundingBox2D({0,0},{1,1}));
    BOOST_CHECK_EQUAL(first_quadrant,BoundingBox2D({-1,0},{0,1}));
    BOOST_CHECK_EQUAL(second_quadrant,BoundingBox2D({0,-1},{1,0}));
    BOOST_CHECK_EQUAL(third_quadrant,BoundingBox2D({-1,-1},{0,0}));
}

BOOST_AUTO_TEST_SUITE_END()