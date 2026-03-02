//
// Created by ace on 2026-02-27.
//
#define BOOST_TEST_DYN_LINK
#include <filesystem>
#include <boost/test/unit_test.hpp>

#include <ocad/ocad_helper.h>

#include "ocad/ocad_importer.h"
#include "ocad/ocad_types_v12.h"

BOOST_AUTO_TEST_SUITE(OcadHelperTestSuite)

using Format = Ocad::OcadTypesV12::Format;

BOOST_AUTO_TEST_CASE(Test_OcadFile_ConstructionBehavingAsExpectedOnEmptyBuffer)
{
    // Ocad::OcadFile<F> file;
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(OcadImporterTestSuite)

class F
{
    public:
    F() = default;
    ~F() = default;
    std::filesystem::path test_map_path_{"./assets/ForestOrienteeringMapBürenflue.ocd"};
    Map map_;
};
using Format = Ocad::OcadTypesV12::Format;

BOOST_FIXTURE_TEST_CASE(Test_OcadImporter_ConstructionBehavingAsExpected,F)
{

    Ocad::OcadImporter imp(test_map_path_,map_);
}

BOOST_AUTO_TEST_SUITE_END()