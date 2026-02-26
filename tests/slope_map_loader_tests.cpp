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

    for (int i = 2; i < 9; ++i)
    {
        for (int j = 2; j < 9; ++j)
        {
            img(i,j) = i * 0.025f + j * 0.025f;
        }
    }
    HeightMap hm(img,1,2.5);

    auto sm = SlopeMapLoader::LoadHeightMap(hm);

    for (int i = 0; i < hm.Dim().x; ++i)
    {
        for (int j = 0; j < hm.Dim().y; ++j)
        {
            BOOST_TEST(hm.SlopeAt({i,j}) == sm.SlopeAt({i,j}));
        }
    }

}

BOOST_AUTO_TEST_SUITE_END()