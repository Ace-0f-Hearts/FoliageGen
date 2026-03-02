//
// Created by ace on 2026-02-25.
//

#define BOOST_TEST_DYN_LINK

#include <cmath>
#include <boost/test/unit_test.hpp>

#include "botanics_math/vec.h"
namespace utf = boost::unit_test;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(VecTestsSuite,*utf::tolerance(std::numeric_limits<float>::epsilon()))

    BOOST_AUTO_TEST_CASE(Test_Vec3_AdditionBehavesAsExpected)
{
    auto x = Vec3(0,1,2);
    auto y = Vec3(2,1,0);

    auto expected = Vec3(2,2,2);
    auto actual = x + y;

    BOOST_TEST(actual == expected,tt::tolerance(0.00001f));
}

BOOST_AUTO_TEST_CASE(Test_Vec3_SubtractionBehavesAsExpected,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    auto x = Vec3(0,1,2);
    auto y = Vec3(2,1,0);

    auto expected = Vec3(-2,0,2);
    auto actual = x - y;

    BOOST_TEST(actual == expected,tt::tolerance(0.00001f));
}

BOOST_AUTO_TEST_CASE(Test_Vec3_MultiplicationBehavesAsExpected,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    auto x = Vec3(0,1,2);
    auto n = 1.5f;

    auto expected = Vec3(0,1.5,3);
    auto actual = x * n;

    BOOST_TEST(actual == expected,tt::tolerance(0.00001f));
}

BOOST_AUTO_TEST_CASE(Test_Vec3_DivisionBehavesAsExpected,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    auto x = Vec3(0,1,2);
    auto n = 2.f;

    auto expected = Vec3(0,0.5,1);
    auto actual = x / n;

    BOOST_TEST(actual == expected,tt::tolerance(0.00001f));
}

BOOST_AUTO_TEST_CASE(Test_Vec3_NormalizationBehavesAsExpected,*utf::tolerance(0.0001f))
{
    auto x = Vec3(0,1,2);

    auto expected = Vec3(0,1.f/std::sqrt(5.f),2.f/std::sqrt(5.f));
    auto actual = Normalize(x);

    BOOST_TEST(actual == expected,tt::tolerance(0.0001f));
}

BOOST_AUTO_TEST_CASE(Test_Vec3_NormalCalculatedCorrectly,*utf::tolerance(0.0001f))
{
    auto x = Vec3(0,1,2);

    auto expected = std::sqrt(5.f);
    auto actual = Normal(x);

    BOOST_TEST(actual == expected,tt::tolerance(0.0001f));
}

BOOST_AUTO_TEST_SUITE_END()