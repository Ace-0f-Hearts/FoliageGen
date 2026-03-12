//
// Created by ace on 2026-02-26.
//

#define BOOST_TEST_DYN_LINK

#define cimg_use_jpeg 1
#define cimg_use_png 1
#define cimg_display 0
#include <CImg.h>
#include <boost/test/unit_test.hpp>

#include "foliage/height_map.h"
#include "foliage/slope_map_loader.h"

using namespace cimg_library;
BOOST_AUTO_TEST_SUITE(SlopeMapLoaderTestSuite)

BOOST_AUTO_TEST_CASE(Test_SlopeMapLoader_LoadingFromHeightMap)
{
    CImg<> img = CImg<>(11,11,1,1,0);

    float i_offset = 2;
    float j_offset = 2;

    for (int i = 2; i < 9; ++i)
    {
        for (int j = 2; j < 9; ++j)
        {
            img(i,j) = i_offset * 0.025f + j_offset * 0.025f;
            j_offset += 1.f;
        }
        i_offset += 1.f;
    }
    HeightMap hm(img,1,2.5);

    auto sm = SlopeMapLoader::Load(hm);

    auto expected_equal = true;
    auto actually_equal = true;

    for (int i = 0; i < hm.Dim().x && actually_equal; ++i)
    {
        for (int j = 0; j < hm.Dim().y && actually_equal; ++j)
        {
            actually_equal = hm.SlopeAt({i,j}) == sm.SlopeAt({i,j});
        }
    }
    BOOST_TEST(expected_equal == actually_equal);

}

BOOST_AUTO_TEST_SUITE_END()