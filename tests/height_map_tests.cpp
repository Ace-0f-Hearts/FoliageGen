//
// Created by ace on 2026-02-25.
//
#define BOOST_TEST_DYN_LINK

#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0
#include <CImg.h>
#include <boost/test/unit_test.hpp>
#include <pstl/algorithm_impl.h>

#include "foliage/height_map.h"


namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(HeightMapTests)

BOOST_AUTO_TEST_CASE(Test_HeightMap_ConstrWithNonEmptyImageSucceededing)
{
    cimg_library::CImg<> img(11,11,1,1,0.f);

    BOOST_CHECK_NO_THROW(HeightMap hm(img););

}

BOOST_AUTO_TEST_CASE(Test_HeightMap_ConstrWithEmptyImageFailing)
{
    cimg_library::CImg<> img;

    BOOST_CHECK_THROW({HeightMap hm(img);},std::invalid_argument);

}

BOOST_AUTO_TEST_CASE(Test_HeightMap_HeightQueryBehavesAsExpected)
{
    CImg<> img(11,11,1,1,0.f);
    img(5,5) = 0.9;

    auto hm = HeightMap(img);

    float expected = 0.9;
    auto actual = hm.HeightAt({5,5});
    BOOST_CHECK_EQUAL(actual,expected);
}

BOOST_AUTO_TEST_CASE(Test_HeightMap_SlopeQueryReturnsHalfPi,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    CImg<> img(11,11,1,1,0.f);
    img(5,5) = 0.9;

    auto hm = HeightMap(img);

    float expected = M_PI / 2;
    auto actual = hm.SlopeAt({5,5});
    BOOST_TEST(actual == expected);
}

BOOST_AUTO_TEST_CASE(Test_HeightMap_SlopeQueryReturnsQuarterPi,*utf::tolerance(0.000001f))
{
    CImg<> img(11,11,1,1,0.f);
    img(5,5) = 1.0f;
    img(6,5) = 0.5f;
    img(7,5) = 0.0f;
    img(6,4) = 0.5f;
    img(6,6) = 0.5f;

    auto hm = HeightMap(img,1,2);

    float expected = M_PI / 4.f;
    auto actual = hm.SlopeAt({6,5});
    BOOST_TEST(actual == expected);
}

BOOST_AUTO_TEST_CASE(Test_HeightMap_NormalQueryReturnsVectorPointingUp,*utf::tolerance(std::numeric_limits<float>::epsilon()))
{
    CImg<> img(11,11,1,1,0.f);
    img(5,5) = 1.0f;

    auto hm = HeightMap(img,1,2);

    auto expected = Vec3(0.f,-1.f,0.f);
    auto actual = hm.NormalAt({5,5});

    BOOST_TEST(actual == expected);
}

BOOST_AUTO_TEST_CASE(Test_HeightMap_NormalQueryReturnsCorrectVector,*utf::tolerance(0.0001f))
{
    CImg<> img(11,11,1,1,0.f);
    img(5,5) = 1.0f;
    img(6,5) = 0.5f;
    img(7,5) = 0.0f;
    img(6,4) = 0.5f;
    img(6,6) = 0.5f;

    auto hm = HeightMap(img,1,2);


    auto expected = Normalize(Vec3(-M_PI / 4,-M_PI / 4,0.f));
    auto actual = hm.NormalAt({6,5});

    BOOST_TEST(actual.x == expected.x);
    BOOST_TEST(actual.y == expected.y);
    BOOST_TEST(actual.z == expected.z);

}
BOOST_AUTO_TEST_SUITE_END()
