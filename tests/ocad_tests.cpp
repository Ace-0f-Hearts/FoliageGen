//
// Created by ace on 2026-02-27.
//
#define BOOST_TEST_DYN_LINK
#include <filesystem>
#include <boost/test/unit_test.hpp>

#include <ocad/ocad_helper.h>

#include "ocad/file_format_registry.h"
#include "ocad/ocad_importer.h"
#include "ocad/ocad_types_v12.h"
using Format = Ocad::OcadTypesV12::Format;


BOOST_AUTO_TEST_SUITE(FileFormatRegistryTestSuite)

class F
{
public:
    F() = default;
    ~F() = default;
    std::filesystem::path test_map_path_{"./assets/SinglePathExample.ocd"};
    std::filesystem::path test_gibberish_{"./assets/gibberish.ocd"};
    std::filesystem::path test_empty_{"./assets/empty.ocd"};
    std::filesystem::path test_wrong_format_wrong_content_{"./assets/empty.txt"};
    std::filesystem::path test_wrong_format_correct_content_{"./assets/incorrectFormat.ocad"};
    std::shared_ptr<OrienteeringMap> map_ = std::make_shared<OrienteeringMap>();
};
BOOST_FIXTURE_TEST_CASE(Test_FileFormatRegistry_FileFormatCorrectlyDeduced,F)
{
    FileFormatRegistry registry{};
    BOOST_CHECK(registry.CreateImporter(test_map_path_,map_, {}) != nullptr);
}

BOOST_FIXTURE_TEST_CASE(Text_FileFormatRegistry_CorrectExtensionButEmptyFileProducesNothing,F)
{
    FileFormatRegistry registry{};
    BOOST_CHECK(registry.CreateImporter(test_empty_,map_, {}) == nullptr);
}

BOOST_FIXTURE_TEST_CASE(Text_FileFormatRegistry_CorrectExtensionButBadContentProducesNothing,F)
{
    FileFormatRegistry registry{};
    BOOST_CHECK(registry.CreateImporter(test_gibberish_,map_, {}) == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(OcadHelperTestSuite)
class F
{
public:
    F() = default;
    ~F() = default;
    std::filesystem::path test_map_path_{"./assets/SinglePathExample.ocd"};
    std::filesystem::path test_gibberish_{"./assets/gibberish.ocd"};
    std::filesystem::path test_empty_{"./assets/empty.ocd"};
    std::filesystem::path test_wrong_format_wrong_content_{"./assets/empty.txt"};
    std::filesystem::path test_wrong_format_correct_content_{"./assets/incorrectFormat.ocad"};
    std::shared_ptr<OrienteeringMap> map_ = std::make_shared<OrienteeringMap>();
};

BOOST_FIXTURE_TEST_CASE(Test_OcadImporter_ConstructionBehavingAsExpected,F)
{
    Ocad::OcadImporter imp(test_map_path_,map_, {});

    imp.DoImport();

    std::cout << map_->GetObjectAmount() << std::endl;
    std::cout << map_->GetSymbolAmount() << std::endl;
    // BOOST_CHECK_EQUAL();
}

BOOST_AUTO_TEST_SUITE_END()